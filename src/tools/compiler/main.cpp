
#pragma warning(disable:4786)

#include "libs.h"

#include <iostream>

#include "../../config.h"
#include "../../stdutil/stdutil.h"

#include <set>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "target.h"
#include "assem_x86/assem_x86.h"
#include "codegen_x86/codegen_x86.h"
#include "../legacy/debugger/debugger.h"

#include <toml.hpp>

#ifdef USE_LLVM
#include "codegen_llvm/codegen_llvm.h"
#include "linker_lld/linker_lld.h"
#include "jit_orc/jit_orc.h"
#endif

#if defined(WIN32) && !defined(__MINGW32__)
#define DEBUGGER "debugger"
#else
#define DEBUGGER "debugger.console"
#endif

#ifdef WIN32
#include <StackWalker.h>

class MyStackWalker:public StackWalker{
public:
	MyStackWalker():StackWalker(){}
protected:
	virtual void OnOutput( LPCSTR szText ){
		fprintf( stderr,"%s",szText );StackWalker::OnOutput( szText );
	}
};

static long WINAPI exceptionFilter( EXCEPTION_POINTERS *e ){
	MyStackWalker sw; sw.ShowCallstack( GetCurrentThread(),e->ContextRecord );
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif

#ifdef BB_POSIX
#include <unistd.h>
#include <execinfo.h>
#include <signal.h>
#include <dirent.h>

#define TRACE_DEPTH 100
static
void print_trace() {
	void *array[TRACE_DEPTH];
	char **strings;
	int size, i;

	size = backtrace( array,TRACE_DEPTH );
	strings = backtrace_symbols( array,size );
	if( strings!=NULL ){
		fprintf(stderr, "Obtained %d stack frames.\n", size);
		for (i = 0; i < size; i++) {
			fprintf(stderr, "%s\n", strings[i]);
		}
	}

	free(strings);
}

static void handle_abort(int sig) {
	fprintf(stderr, "signal: abort\n");
	print_trace();
	exit(1);
}

static void handle_segfault(int sig) {
	fprintf(stderr, "signal: segfault\n");
	print_trace();
	exit(1);
}
#endif

static void showInfo(){
	const int major=(VERSION&0xffff)/100,minor=(VERSION&0xffff)%100;
	std::cout<<"Blitz3D-NG V"<<major<<"."<<std::setfill('0')<<std::setw(2)<<minor<<std::endl;
}

static void showUsage(){
	std::cout<<"Usage: blitzcc [-h|-q|+q|-c|-d|-k|+k|-v|-o exefile] [sourcefile.bb]"<<std::endl;
}

static void showHelp(){
	showUsage();
	std::cout<<"-a         : dump asm"<<std::endl;
	std::cout<<"-h         : show this help"<<std::endl;
	std::cout<<"-q         : quiet mode"<<std::endl;
	std::cout<<"+q         : very quiet mode"<<std::endl;
	std::cout<<"-c         : compile only"<<std::endl;
	std::cout<<"-d         : debug compile"<<std::endl;
	std::cout<<"-j         : dump json ast"<<std::endl;
	std::cout<<"-k         : dump keywords"<<std::endl;
	std::cout<<"+k         : dump keywords and syntax"<<std::endl;
	std::cout<<"-llvm      : use llvm"<<std::endl;
	std::cout<<"-r         : list available runtimes"<<std::endl;
	std::cout<<"-v         : version info"<<std::endl;
	std::cout<<"-o exefile : generate executable"<<std::endl;

}

static void err( const std::string &t ){
	std::cout<<t<<std::endl;
	exit(-1);
}

static void usageErr(){
	err( "Usage error" );
}

static std::string quickHelp( const std::string &kw ){

	Environ *e=runtimeEnviron;
	Decl *d=e->funcDecls->findDecl( tolower( kw ) );
	if( !d || d->type->funcType()==0 ) return "No quick help available for "+kw;
	std::string t=kw;
	FuncType *f=d->type->funcType();
	if( f->returnType==Type::float_type ) t+='#';
	else if( f->returnType==Type::string_type ) t+='$';

	t+=" ";

	if( f->returnType!=Type::void_type ) t+="( ";

	for( int k=0;k<f->params->size();++k ){
		std::string s;
		if( k ) s+=',';
		Decl *p=f->params->decls[k];s+=p->name;
		if( p->type==Type::float_type ) s+='#';
		else if( p->type==Type::string_type ) s+='$';
		else if( p->type==Type::void_type ) s+='*';
		if( p->defType ) s='['+s+']';
		t+=s;
	}

	if( f->returnType!=Type::void_type ){
		t+=f->params->size() ? " )" : ")";
	}
	return t;
}

static void dumpKeys( bool lang,bool mod,bool help ){

	if( lang ){
		std::map<std::string,int>::iterator it;
		std::map<std::string,int> &keywords=Toker::getKeywords();
		for( it=keywords.begin();it!=keywords.end();++it ){
			if( it->first.find(' ')!=std::string::npos ) continue;
			std::cout<<it->first<<std::endl;
		}
	}

	if( !mod ) return;

	for( int k=0;k<keyWords.size();++k ){
		std::string t=keyWords[k];

		if( t[0]=='_' )	continue;
		if( !isalpha( t[0] ) ) t=t.substr( 1 );
		for( int n=0;n<t.size();++n ){
			if( !isalnum(t[n]) && t[n]!='_' ){
				t=t.substr( 0,n );
				break;
			}
		}
		if( help ) t=quickHelp(t);
		std::cout<<t<<std::endl;
	}
}

static std::string verstr( int ver ){
	return itoa((ver&65535)/100)+"."+itoa((ver&65535)%100);
}

static void versInfo(){
	std::cout<<"Compiler version:"<<verstr(bcc_ver)<<std::endl;
	std::cout<<"Runtime version:"<<verstr(run_ver)<<std::endl;
	std::cout<<"Debugger version:"<<verstr(dbg_ver)<<std::endl;
	std::cout<<"Linker version:"<<verstr(lnk_ver)<<std::endl;
}

static void demoError(){
	std::cout<<"Compiler can not be used standalone in demo version."<<std::endl;
	exit(0);
}

#ifndef WIN32
void enumToolchainFiles( const std::string &binpath,std::vector<std::string> &paths ){
	DIR *bindir=opendir( binpath.c_str() );
	if( !bindir ) return;

	struct dirent *ent;
	while( (ent=readdir( bindir ))!=NULL ){
		if( ent->d_type==DT_DIR && std::string(ent->d_name)!=std::string(".") && std::string(ent->d_name)!=std::string("..") ){
			std::string toolpath=binpath+"/"+ent->d_name;
			DIR *tooldir=opendir( toolpath.c_str() );
			while( (ent=readdir( tooldir ))!=NULL ){
				if( ent->d_type==DT_REG && std::string(ent->d_name)=="toolchain.toml" ){
					paths.push_back( toolpath+"/"+ent->d_name );
				}
			}
			closedir( tooldir );
		}
	}

	closedir( bindir );
}
#else
void enumToolchainFiles( const std::string &binpath,vector<string> &paths ){
	WIN32_FIND_DATA bindata;
	HANDLE bindir;
	bindir=FindFirstFile( (binpath+"/*").c_str(),&bindata );
	do{
		string toolname( bindata.cFileName );
		if( (bindata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && toolname.length()>0 ){
			string toolpath=canonicalpath( binpath+"/"+toolname+"/toolchain.toml" );
			if( !(GetFileAttributes( toolpath.c_str() )&FILE_ATTRIBUTE_DIRECTORY) ){
				paths.push_back( toolpath );
			}
		}
	}while( FindNextFile( bindir,&bindata )!=0 );
	FindClose( bindir );
}
#endif

const char *enumTargets( std::vector<Target> &targets ){
	char *p=getenv( "blitzpath" );
	if( !p ) return "Can't find blitzpath environment variable";
	std::string home=p;

	std::vector<std::string> paths;
	enumToolchainFiles( home+"/bin",paths );

	for( const std::string &toolpath:paths ){
		auto data=toml::parse( toolpath );
		const std::string triple=toml::find<std::string>( data,"id" );
		const std::string platform=toml::find<std::string>( data,"platform" );
		const std::string platform_version=toml::find<std::string>( data,"platform_version" );
		const std::string arch=toml::find<std::string>( data,"arch" );

		Target t=Target( triple,platform,arch,platform_version );

		for( const auto &ent:toml::find( data,"runtime" ).as_table() ){
			Target::Runtime rt;
			rt.id=ent.first;
			rt.entry=toml::find<std::string>( ent.second,"entry" );;
			rt.modules=toml::find<std::vector<std::string>>( ent.second,"deps" );
			t.runtimes.insert( std::pair<std::string,Target::Runtime>( ent.first,rt ) );
		}

		for( const auto &ent:toml::find( data,"module" ).as_table() ){
			Target::Module mod;
			mod.id=ent.first;
			if( ent.second.contains("libs") ) mod.libs=toml::find<std::vector<std::string>>( ent.second,"libs" );
			if( ent.second.contains("system_libs") ) mod.system_libs=toml::find<std::vector<std::string>>( ent.second,"system_libs" );
			if( ent.second.contains("extra_files") ) mod.extra_files=toml::find<std::vector<std::string>>( ent.second,"extra_files" );
			if( ent.second.contains("symbols") ) mod.symbols=toml::find<std::vector<std::string>>( ent.second,"symbols" );
			t.modules.insert( std::pair<std::string,Target::Module>( ent.first,mod ) );
		}

		targets.push_back( t );
	}

	return 0;
}

#ifdef WIN32
static const char *enumRuntimes( vector<string> &rts ){
	char *p=getenv( "blitzpath" );
	if( !p ) return "Can't find blitzpath environment variable";
	string home=string(p);

	WIN32_FIND_DATA ffd;
	HANDLE find;
	find=FindFirstFile( (home+"/bin/" RUNTIMENAME ".*.dll").c_str(),&ffd );
	do{
		string fname(ffd.cFileName);
		if( !(ffd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && fname.length()>0 ){
			fname=fname.substr( strlen(RUNTIMENAME)+1 );
			fname=fname.substr( 0,fname.length()-4 );
			rts.push_back( fname );
		}
	} while (FindNextFile(find, &ffd) != 0);
	FindClose(find);

	if( rts.size() == 0 ) return "No runtimes found";
	return 0;
}
#endif

int main( int argc,char *argv[] ){
#if defined(BB_POSIX) && !defined(BB_ASAN)
	signal(SIGABRT, handle_abort);
	signal(SIGSEGV, handle_segfault);
#endif

#ifdef WIN32
	SetUnhandledExceptionFilter( exceptionFilter );
#endif

	std::string in_file,out_file,rt,args,targetid,signerId,teamId;

	bool debug=false,quiet=false,veryquiet=false,compileonly=false;
	bool dumpkeys=false,dumphelp=false,showhelp=false,dumpasm=false,dumptree=false;
#if defined(USE_LLVM) && !defined(BB_WIN32)
	bool usellvm=true;
#else
	bool usellvm=false;
#endif
	bool versinfo=false,rtinfo=false;

	for( int k=1;k<argc;++k ){

		std::string t=argv[k];

		t=tolower(t);

		if( t=="-h" ){
			showhelp=true;
		}else if( t=="-a" ){
			dumpasm=true;
		}else if( t=="-q" ){
			quiet=true;
		}else if( t=="+q" ){
			quiet=veryquiet=true;
		}else if( t=="-c" ){
			compileonly=true;
		}else if( t=="-d" ){
			debug=true;
		}else if( t=="-j" ){
			dumptree=true;
		}else if( t=="-k" ){
			dumpkeys=true;
		}else if( t=="+k" ){
			dumpkeys=dumphelp=true;
		}else if( t=="-llvm" ){
			usellvm=true;
		}else if( t=="-llvm=off" ){
			usellvm=false;
		}else if( t=="-v" ){
			versinfo=true;
		}else if( t=="-e" ){
			std::cout<<BB_ENV<<std::endl;
			return 0;
		}else if( t=="-l" ){
			rtinfo=true;
		}else if( t=="-r" ){
			if( out_file.size() || k==argc-1 ) usageErr();
			rt=argv[++k];
		}else if( t=="-o" ){
			if( out_file.size() || k==argc-1 ) usageErr();
			out_file=argv[++k];
		}else if( t=="-target" ){
			if( out_file.size() || k==argc-1 ) usageErr();
			targetid=argv[++k];
		}else if( t=="-sign" ){
			if( out_file.size() || k==argc-1 ) usageErr();
			signerId=argv[++k];
		}else if( t=="-team" ){
			if( out_file.size() || k==argc-1 ) usageErr();
			teamId=argv[++k];
		}else{
			if( in_file.size() || t[0]=='-' || t[0]=='+' ) usageErr();
			in_file=argv[k];
			for( ++k;k<argc;++k ){
				std::string t=argv[k];
				if( t.find(' ')!=std::string::npos ) t='\"'+t+'\"';
				if( args.size() ) args+=' ';
				args+=t;
			}
		}
	}

	std::vector<Target> targets;
	if( const char *er=enumTargets( targets ) ) err( er );

	std::vector<std::string> rts;
#ifdef WIN32
	if( const char *er=enumRuntimes( rts ) ) err( er );

	if( rtinfo ){
		if( !quiet ) cout<<"Found "<<rts.size()<<" runtimes:"<<std::endl;
		for ( unsigned i=0;i<rts.size();i++ ){
			std::cout<<rts[i]<<std::endl;
		}
		return 0;
	}

	if( rt=="" ) rt=rts[0];

	if( std::find( std::begin(rts),std::end(rts),rt )==std::end(rts) ){
		err( ("Invalid runtime: "+rt).c_str() );
	}
#else
	if( rt == "" ){
		rt="opengl";
	}
#endif

	int ti=-1;
	for( int i=0;i<targets.size();i++ ){
		if(
			(targetid.size()==0&&targets[i].host)||
			(targetid=="host"&&targets[i].host)||
			(targetid==targets[i].triple||targetid==targets[i].type)
		){
			ti=i;
			break;
		}
	}

	if( ti==-1 ) err( "Invalid target" );

	Target target=targets[ti];
	if( !target.host ){
		compileonly=true;
	}

	if( out_file.size() && !in_file.size() ) usageErr();

	if( const char *er=openLibs( target,rt ) ) err( er );

	if( const char *er=linkLibs( target,rt ) ) err( er );

	if( showhelp ) showHelp();
	if( dumpkeys ) dumpKeys( true,true,dumphelp );
	if( versinfo ) versInfo();

	if( !in_file.size() ) return 0;

#ifndef USE_LLVM
	if( usellvm ) {
		cerr<<"not compiled with llvm support"<<std::endl;
		return 0;
	}
#endif

#ifdef DEMO
	if( !getenv( "blitzide" ) ) demoError();
#endif

	if( in_file[0]=='\"' ){
		if( in_file.size()<3 || in_file[in_file.size()-1]!='\"' ) usageErr();
		in_file=in_file.substr( 1,in_file.size()-2 );
	}

	std::ifstream in( in_file.c_str() );
	if( !in ) err( "Unable to open input file" );
	if( !quiet ){
		showInfo();
		std::cout<<"Compiling \""<<in_file<<"\""<<std::endl;
	}

	int n=in_file.rfind( '/' );
	if( n==std::string::npos ) n=in_file.rfind( '\\' );
	if( n!=std::string::npos ){
		if( !n || in_file[n-1]==':' ) ++n;
		chdir( in_file.substr(0,n).c_str() );
	}

	ProgNode *prog=0;
	Environ *env=0;
	Module *module=0;
	BundleInfo bundle;

#ifdef USE_LLVM
	std::string obj_code;
#endif

	try{
		//parse
		if( !veryquiet ) std::cout<<"Parsing..."<<std::endl;
		Toker toker( in );
		Parser parser( toker );
		prog=parser.parse( in_file );

		bundle=parser.bundle;
		bundle.signerId = signerId;
		bundle.teamId = teamId;

		//semant
		if( !veryquiet ) std::cout<<"Generating..."<<std::endl;
		env=prog->semant( runtimeEnviron );

		if( dumptree ){
			std::cout<<prog->toJSON( debug ).dump(2)<<std::endl;
			return 0;
		}

		//translate
		if( !veryquiet ) std::cout<<"Translating..."<<std::endl;

		qstreambuf qbuf;
		std::iostream asmcode( &qbuf );
		Codegen_x86 codegen( asmcode,debug );
#ifdef USE_LLVM
		Codegen_LLVM codegen2( debug );
		codegen2.SetTarget( target );
#endif

		if ( usellvm ) {
#ifdef USE_LLVM
			prog->translate2( &codegen2,userFuncs );

			if( out_file.size() ){
				codegen2.injectMain();
			}
			codegen2.optimize();

			if( dumpasm ){
				codegen2.dumpToStderr();
			}
#endif
		} else {
			prog->translate( &codegen,userFuncs );

			if( dumpasm ){
				std::cout<<std::endl<<std::string( qbuf.data(),qbuf.size() )<<std::endl;
			}
		}

		if( bundle.enabled && out_file.size() && target.host ){
#ifdef BB_MACOS
			out_file+=".app";
#endif
		}

		//assemble
		if( !veryquiet ) std::cout<<"Assembling..."<<std::endl;

		if ( usellvm ) {
#ifdef USE_LLVM
			codegen2.dumpToObj( obj_code );
#endif
		} else {
			module=linkerLib->createModule();
			Assem_x86 assem( asmcode,module );
			assem.assemble();
		}
	}catch( Ex &x ){
		std::string file='\"'+x.file+'\"';
		int row=((x.pos>>16)&65535)+1,col=(x.pos&65535)+1;
		std::cout<<file<<":"<<row<<":"<<col<<":"<<row<<":"<<col<<":"<<x.ex<<std::endl;
		exit(-1);
	}

	delete prog;

	int ret=0;

	if( out_file.size() ){
		if( !veryquiet ) std::cout<<"Creating "+std::string(bundle.enabled?"bundle":"executable")+" \""<<out_file<<"\"..."<<std::endl;
		if( usellvm ) {
#ifdef USE_LLVM
			Linker_LLD linker( home );
			linker.createExe( rt,target,obj_code,bundle,out_file );
#else
			cerr<<"llvm support was not compiled in"<<std::endl;
			abort();
#endif
		}else{
#ifdef WIN32
			char buff[MAX_PATH];
			GetFullPathName( out_file.c_str(),MAX_PATH,buff,NULL );
			if( !module->createExe( buff,(home+"/bin/runtime."+rt+".dll").c_str() ) ){
				err( "Error creating executable" );
			}
#else
			std::cerr<<"you must use llvm on non-Windows platforms"<<std::endl;
			abort();
#endif
		}
	}else if( !compileonly ){
		void *entry=0;
		Debugger *debugger=0;

		if ( usellvm ) {
#ifdef USE_LLVM
			if( !veryquiet ) std::cout<<"Executing..."<<std::endl;

			ret=JIT_ORC::run( runtimeLib,obj_code,home,rt );
#else
			std::cerr<<"llvm support was not compiled in"<<std::endl;
			abort();
#endif
		} else {
			entry=module->link( runtimeModule );
		}

		if( !entry ) return ret;

#ifdef WIN32
		HMODULE dbgHandle=0;

		if( debug ){
			dbgHandle=LoadLibrary( (home+"/bin/" DEBUGGER ".dll").c_str() );
			if( dbgHandle ){
				typedef Debugger *(__cdecl*GetDebugger)( Module*,Environ* );
				GetDebugger gd=(GetDebugger)GetProcAddress( dbgHandle,"debuggerGetDebugger" );
				if( gd ) debugger=gd( module,env );
			}
			if( !debugger ) err( "Error launching debugger" );
		}
#endif

		if( !veryquiet ) std::cout<<"Executing..."<<std::endl;

		ret=runtimeLib->execute( (void(*)())entry,args.c_str(),debugger );
#ifdef WIN32
		if( dbgHandle ) FreeLibrary( dbgHandle );
#endif
	}

	delete module;
	delete env;

	closeLibs();

	return ret;
}
