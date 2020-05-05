
#pragma warning(disable:4786)

#include "libs.h"

#include <iostream>

#include "../../../config/config.h"
#include "../../../stdutil/stdutil.h"

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

using namespace std;

#include "assem_x86/assem_x86.h"
#include "codegen_x86/codegen_x86.h"

#ifdef USE_LLVM
#include "codegen_llvm/codegen_llvm.h"
#include "linker_lld/linker_lld.h"
#endif

#if defined(WIN32) && !defined(__MINGW32__)
#define DEBUGGER "debugger"
#else
#define DEBUGGER "debugger.console"
#endif

static void showInfo(){
	const int major=(VERSION&0xffff)/100,minor=(VERSION&0xffff)%100;
	cout<<"Blitz3D-NG V"<<major<<"."<<setfill('0')<<setw(2)<<minor<<endl;
}

static void showUsage(){
	cout<<"Usage: blitzcc [-h|-q|+q|-c|-d|-k|+k|-v|-o exefile] [sourcefile.bb]"<<endl;
}

static void showHelp(){
	showUsage();
	cout<<"-a         : dump asm"<<endl;
	cout<<"-h         : show this help"<<endl;
	cout<<"-q         : quiet mode"<<endl;
	cout<<"+q         : very quiet mode"<<endl;
	cout<<"-c         : compile only"<<endl;
	cout<<"-d         : debug compile"<<endl;
	cout<<"-j         : dump json ast"<<endl;
	cout<<"-k         : dump keywords"<<endl;
	cout<<"+k         : dump keywords and syntax"<<endl;
	cout<<"-llvm      : use llvm"<<endl;
	cout<<"-r         : list available runtimes"<<endl;
	cout<<"-v         : version info"<<endl;
	cout<<"-o exefile : generate executable"<<endl;

}

static void err( const string &t ){
	cout<<t<<endl;
	exit(-1);
}

static void usageErr(){
	err( "Usage error" );
}

static string quickHelp( const string &kw ){

	Environ *e=runtimeEnviron;
	Decl *d=e->funcDecls->findDecl( tolower( kw ) );
	if( !d || d->type->funcType()==0 ) return "No quick help available for "+kw;
	string t=kw;
	FuncType *f=d->type->funcType();
	if( f->returnType==Type::float_type ) t+='#';
	else if( f->returnType==Type::string_type ) t+='$';

	t+=" ";

	if( f->returnType!=Type::void_type ) t+="( ";

	for( int k=0;k<f->params->size();++k ){
		string s;
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
		map<string,int>::iterator it;
		map<string,int> &keywords=Toker::getKeywords();
		for( it=keywords.begin();it!=keywords.end();++it ){
			if( it->first.find(' ')!=string::npos ) continue;
			cout<<it->first<<endl;
		}
	}

	if( !mod ) return;

	for( int k=0;k<keyWords.size();++k ){
		string t=keyWords[k];

		if( t[0]=='_' )	continue;
		if( !isalpha( t[0] ) ) t=t.substr( 1 );
		for( int n=0;n<t.size();++n ){
			if( !isalnum(t[n]) && t[n]!='_' ){
				t=t.substr( 0,n );
				break;
			}
		}
		if( help ) t=quickHelp(t);
		cout<<t<<endl;
	}
}

static string verstr( int ver ){
	return itoa((ver&65535)/100)+"."+itoa((ver&65535)%100);
}

static void versInfo(){
	cout<<"Compiler version:"<<verstr(bcc_ver)<<endl;
	cout<<"Runtime version:"<<verstr(run_ver)<<endl;
	cout<<"Debugger version:"<<verstr(dbg_ver)<<endl;
	cout<<"Linker version:"<<verstr(lnk_ver)<<endl;
}

static void demoError(){
	cout<<"Compiler can not be used standalone in demo version."<<endl;
	exit(0);
}

#ifdef WIN32
static const char *enumRuntimes( vector<string> &rts ){
	char *p=getenv( "blitzpath" );
	if( !p ) return "Can't find blitzpath environment variable";
	string home=string(p);

	WIN32_FIND_DATA ffd;
	HANDLE find;
	find=FindFirstFile( (home+"/bin/runtime.*.dll").c_str(),&ffd );
	do{
		string fname(ffd.cFileName);
		if( !(ffd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && fname.length()>0 ){
			fname=fname.substr( 8 );
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

	string in_file,out_file,rt,args;
	vector<string> rts;

	bool debug=false,quiet=false,veryquiet=false,compileonly=false;
	bool dumpkeys=false,dumphelp=false,showhelp=false,dumpasm=false,dumptree=false;
	bool usellvm=false;
	bool versinfo=false,rtinfo=false;

	for( int k=1;k<argc;++k ){

		string t=argv[k];

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
		}else if( t=="-v" ){
			versinfo=true;
		}else if( t=="-e" ){
			cout<<BB_ENV<<endl;
			return 0;
		}else if( t=="-l" ){
			rtinfo=true;
		}else if( t=="-r" ){
			if( out_file.size() || k==argc-1 ) usageErr();
			rt=argv[++k];
		}else if( t=="-o" ){
			if( out_file.size() || k==argc-1 ) usageErr();
			out_file=argv[++k];
		}else{
			if( in_file.size() || t[0]=='-' || t[0]=='+' ) usageErr();
			in_file=argv[k];
			for( ++k;k<argc;++k ){
				string t=argv[k];
				if( t.find(' ')!=string::npos ) t='\"'+t+'\"';
				if( args.size() ) args+=' ';
				args+=t;
			}
		}
	}

#ifdef WIN32
	if( const char *er=enumRuntimes( rts ) ) err( er );

	if( rtinfo ){
		if( !quiet ) cout<<"Found "<<rts.size()<<" runtimes:"<<endl;
		for ( unsigned i=0;i<rts.size();i++ ){
			cout<<rts[i]<<endl;
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

	if( out_file.size() && !in_file.size() ) usageErr();

	if( const char *er=openLibs( rt ) ) err( er );

	if( const char *er=linkLibs( rt ) ) err( er );

	if( showhelp ) showHelp();
	if( dumpkeys ) dumpKeys( true,true,dumphelp );
	if( versinfo ) versInfo();

	if( !in_file.size() ) return 0;

#ifdef DEMO
	if( !getenv( "blitzide" ) ) demoError();
#endif

	if( in_file[0]=='\"' ){
		if( in_file.size()<3 || in_file[in_file.size()-1]!='\"' ) usageErr();
		in_file=in_file.substr( 1,in_file.size()-2 );
	}

	ifstream in( in_file.c_str() );
	if( !in ) err( "Unable to open input file" );
	if( !quiet ){
		showInfo();
		cout<<"Compiling \""<<in_file<<"\""<<endl;
	}

	int n=in_file.rfind( '/' );
	if( n==string::npos ) n=in_file.rfind( '\\' );
	if( n!=string::npos ){
		if( !n || in_file[n-1]==':' ) ++n;
		chdir( in_file.substr(0,n).c_str() );
	}

	ProgNode *prog=0;
	Environ *env=0;
#ifdef WIN32
	Module *module=0;
#endif

	try{
		//parse
		if( !veryquiet ) cout<<"Parsing..."<<endl;
		Toker toker( in );
		Parser parser( toker );
		prog=parser.parse( in_file );

		//semant
		if( !veryquiet ) cout<<"Generating..."<<endl;
		env=prog->semant( runtimeEnviron );

		//translate
		if( !veryquiet ) cout<<"Translating..."<<endl;

		qstreambuf qbuf;
		iostream asmcode( &qbuf );
		Codegen_x86 codegen( asmcode,debug );
#ifdef USE_LLVM
		Codegen_LLVM codegen2( debug );
#endif

		if ( usellvm ) {
#ifdef USE_LLVM
			prog->translate2( &codegen2,userFuncs );

			if( dumpasm ){
				codegen2.dumpToStderr();
			}
#else
			cerr<<"not compiled with llvm support"<<endl;
			abort();
#endif
		} else {
			prog->translate( &codegen,userFuncs );

			if( dumpasm ){
				cout<<endl<<string( qbuf.data(),qbuf.size() )<<endl;
			}
		}

		if( dumptree ){
			cout<<prog->toJSON( debug ).dump(2)<<endl;
		}

		//assemble
		if( !veryquiet ) cout<<"Assembling..."<<endl;

		if ( usellvm ) {
#ifdef USE_LLVM
			codegen2.dumpToObj( home+"/output.o" );
#else
			cerr<<"not compiled with llvm support"<<endl;
			abort();
#endif
		} else {
#ifdef WIN32
			module=linkerLib->createModule();
			Assem_x86 assem( asmcode,module );
			assem.assemble();
#endif
		}
	}catch( Ex &x ){

		string file='\"'+x.file+'\"';
		int row=((x.pos>>16)&65535)+1,col=(x.pos&65535)+1;
		cout<<file<<":"<<row<<":"<<col<<":"<<row<<":"<<col<<":"<<x.ex<<endl;
		exit(-1);
	}

	delete prog;

	if( out_file.size() ){
		if( !veryquiet ) cout<<"Creating executable \""<<out_file<<"\"..."<<endl;
		if( usellvm ) {
#ifdef USE_LLVM
			Linker_LLD linker( home );
			linker.createExe( out_file );
#else
			cerr<<"llvm support was not compiled in"<<endl;
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
			cerr<<"you must use llvm on non-Windows platforms"<<endl;
			abort();
#endif
		}
	}else if( !compileonly ){
#ifdef WIN32
		void *entry=module->link( runtimeModule );
		if( !entry ) return 0;

		HMODULE dbgHandle=0;
		Debugger *debugger=0;

		if( debug ){
			dbgHandle=LoadLibrary( (home+"/bin/" DEBUGGER ".dll").c_str() );
			if( dbgHandle ){
				typedef Debugger *(__cdecl*GetDebugger)( Module*,Environ* );
				GetDebugger gd=(GetDebugger)GetProcAddress( dbgHandle,"debuggerGetDebugger" );
				if( gd ) debugger=gd( module,env );
			}
			if( !debugger ) err( "Error launching debugger" );
		}

		if( !veryquiet ) cout<<"Executing..."<<endl;

		runtimeLib->execute( (void(*)())entry,args.c_str(),debugger );

		if( dbgHandle ) FreeLibrary( dbgHandle );
#endif
	}

#ifdef WIN32
	delete module;
#endif
	delete env;

	closeLibs();

	return 0;
}
