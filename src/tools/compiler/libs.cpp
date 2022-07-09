
#include "libs.h"

#ifdef WIN32
#include <windows.h>
#define OPENLIB( path ) LoadLibrary( path )
#define CLOSELIB( handle ) FreeLibrary( handle )
#define LIBSYM( handle,sym ) GetProcAddress( handle,sym );
#define LIBPATH "\\bin\\" RUNTIMENAME "."+rt+"." LIBSUFFIX
#else
#include <dlfcn.h>
#define OPENLIB( path ) dlopen( path,RTLD_LAZY )
#define CLOSELIB( handle ) dlclose( handle )
#define LIBSYM( handle,sym ) dlsym( handle,sym );
#define LIBPATH "/bin/" BB_TRIPLE "/runtime."+rt+"." LIBSUFFIX
typedef void* HMODULE;
#endif

int bcc_ver;
int lnk_ver;
int run_ver;
int dbg_ver;

string home;
Linker *linkerLib;
Runtime *runtimeLib;
BBRuntimeDylib rtdl;

Module *runtimeModule;
Environ *runtimeEnviron;
vector<string> modules;
vector<string> keyWords;
vector<UserFunc> userFuncs;

static HMODULE runtimeHMOD;

static Type *_typeof( int c ){
	switch( c ){
	case '%':return Type::int_type;
	case '#':return Type::float_type;
	case '$':return Type::string_type;
	}
	return Type::void_type;
}

static int curr;
static string text;

static int next( istream &in ){

	text="";

	int t=0;

	for(;;){
		while( isspace( in.peek() ) ) in.get();
		if( in.eof() ) return curr=0;
		t=in.get();if( t!=';' ) break;
		while( !in.eof() && in.get()!='\n' ){}
	}

	if( isalpha(t) ){
		text+=(char)t;
		while( isalnum( in.peek() ) || in.peek()=='_' ) text+=(char)in.get();
		return curr=-1;
	}
	if( t=='\"' ){
		while( in.peek()!='\"' ) text=text+(char)in.get();
		in.get();
		return curr=-2;
	}

	return curr=t;
}

static const char *linkRuntime(string rt){
	while( const char *sym=runtimeLib->nextSym() ){

		string s( sym );

		int pc=runtimeLib->symValue(sym);

		//internal?
		if( s[0]=='_' ){
			runtimeModule->addSymbol( ("_"+s).c_str(),pc );
			continue;
		}

		bool cfunc=false;

		if( s[0]=='!' ){
			cfunc=true;
			s=s.substr(1);
		}

		keyWords.push_back( s );

		//global!
		int start=0,end;
		Type *t=Type::void_type;
		if( !isalpha( s[0] ) ){ start=1;t=_typeof( s[0] ); }
		int k;
		for( k=1;k<s.size();++k ){
			if( !isalnum( s[k] ) && s[k]!='_' ) break;
		}
		end=k;
		DeclSeq *params=d_new DeclSeq();
		string n=s.substr( start,end-start );
		while( k<s.size() ){
			Type *t=_typeof(s[k++]);
			int from=k;
			for( ;isalnum(s[k])||s[k]=='_';++k ){}
			string str=s.substr( from,k-from );
			ConstType *defType=0;
			if( s[k]=='=' ){
				int from=++k;
				if( s[k]=='\"' ){
					for( ++k;s[k]!='\"';++k ){}
					string t=s.substr( from+1,k-from-1 );
					defType=d_new ConstType( t );++k;
				}else{
					if( s[k]=='-' ) ++k;
					for( ;isdigit( s[k] );++k ){}
					if( t==Type::int_type ){
						int n=atoi( s.substr( from,k-from ) );
						defType=d_new ConstType( n );
					}else{
						float n=atof( s.substr( from,k-from ) );
						defType=d_new ConstType( n );
					}
				}
			}
			Decl *d=params->insertDecl( str,t,DECL_PARAM,defType );
		}

		FuncType *f=d_new FuncType( t,params,false,cfunc );
		f->symbol="bb"+n;
		n=tolower(n);
		runtimeEnviron->funcDecls->insertDecl( n,f,DECL_FUNC );
		runtimeModule->addSymbol( ("_f"+n).c_str(),pc );
	}
	return 0;
}

static set<string> _ulibkws;

static const char *loadUserLib( const string &userlib ){

	string t=home+"/userlibs/"+userlib;

	string lib="";
	ifstream in(t.c_str());

	next(in);
	while( curr ){

		if( curr=='.' ){

			if( next(in)!=-1 ) return "expecting identifier after '.'";

			if( text=="lib" ){
				if( next(in)!=-2 ) return "expecting string after lib directive";
				lib=text;

			}else{
				return "unknown decl directive";
			}
			next( in );

		}else if( curr==-1 ){

			if( !lib.size() ) return "function decl without lib directive";

			string id=text;
			string lower_id=tolower(id);

			if( _ulibkws.count( lower_id ) ) return "duplicate identifier";
			_ulibkws.insert( lower_id );

			Type *ty=0;
			switch( next(in) ){
			case '%':ty=Type::int_type;break;
			case '#':ty=Type::float_type;break;
			case '$':ty=Type::string_type;break;
			}
			if( ty ) next(in);
			else ty=Type::void_type;

			DeclSeq *params=d_new DeclSeq();

			if( curr!='(' ) return "expecting '(' after function identifier";
			next(in);
			if( curr!=')' ){
				for(;;){
					if( curr!=-1 ) break;
					string arg=text;

					Type *ty=0;
					switch( next(in) ){
					case '%':ty=Type::int_type;break;
					case '#':ty=Type::float_type;break;
					case '$':ty=Type::string_type;break;
					case '*':ty=Type::null_type;break;
					}
					if( ty ) next(in);
					else ty=Type::int_type;

					ConstType *defType=0;

					Decl *d=params->insertDecl( arg,ty,DECL_PARAM,defType );

					if( curr!=',' ) break;
					next(in);
				}
			}
			if( curr!=')' ) return "expecting ')' after function decl";

			keyWords.push_back( id );

			FuncType *fn=d_new FuncType( ty,params,true,true );

			runtimeEnviron->funcDecls->insertDecl( lower_id,fn,DECL_FUNC );

			if( next(in)==':' ){	//real name?
				next(in);
				if( curr!=-1 && curr!=-2 ) return "expecting identifier or string after alias";
				id=text;
				next(in);
			}

			userFuncs.push_back( UserFunc( lower_id,id,lib ) );
		}
	}
	return 0;
}

static const char *linkUserLibs(){

	_ulibkws.clear();

#ifdef WIN32
	WIN32_FIND_DATA fd;

	HANDLE h=FindFirstFile( (home+"/userlibs/*.decls").c_str(),&fd );

	if( h==INVALID_HANDLE_VALUE ) return 0;

	const char *err=0;

	do{
		if( err=loadUserLib( fd.cFileName ) ){
			static char buf[64];
			sprintf( buf,"Error in userlib '%s' - %s",fd.cFileName,err );
			err=buf;break;
		}

	}while( FindNextFile( h,&fd ) );

	FindClose( h );
#else
	const char *err=0;
#endif

	_ulibkws.clear();

	return err;
}

static char err[255];

const char *openLibs( const string &rt ){

	char *p=getenv( "blitzpath" );
	if( !p ) return "Can't find blitzpath environment variable";
#ifdef WIN32
	char buff[MAX_PATH];
	GetFullPathName( p,MAX_PATH,buff,NULL );
	home=string(buff);
#else
	char buff[PATH_MAX];
	home=realpath( p,buff );
#endif

	linkerLib=linkerGetLinker();

	runtimeHMOD=OPENLIB( (home+LIBPATH).c_str() );
	if( !runtimeHMOD ){
		string msg("Unable to open " RUNTIMENAME "."+rt+".");
		strcpy( err,msg.c_str() );
		return err;
	}

	typedef Runtime *(CDECL*GetRuntime)();
	GetRuntime gr=(GetRuntime)LIBSYM( runtimeHMOD,"runtimeGetRuntime" );
	if( !gr ){
		string msg("Error in " RUNTIMENAME "."+rt+"." LIBSUFFIX);
		strcpy( err,msg.c_str() );
		return err;
	}
	runtimeLib=gr();
	runtimeLib->path=home+LIBPATH;

	bcc_ver=VERSION;
	lnk_ver=linkerLib->version();
	run_ver=runtimeLib->version();

	if( (lnk_ver>>16)!=(bcc_ver>>16) ||
		(run_ver>>16)!=(bcc_ver>>16) ||
		(lnk_ver>>16)!=(bcc_ver>>16) ) return "Library version error";

	runtimeLib->startup();
	runtimeModule=linkerLib->createModule();
	runtimeEnviron=d_new Environ( "",Type::int_type,0,0 );

	keyWords.clear();
	userFuncs.clear();

	return 0;
}

static
void parse_list( std::string &line,std::vector<string> &libs ){
	size_t s=0,e=line.find(";");
	while( 1 ) {
		string frag=line.substr( s,e-s );
		while( frag[0] == ' ' ) frag=frag.substr( 1 );

		if( frag.length() == 0 ) break;

		libs.push_back( frag );

		if( e==string::npos ) {
			break;
		}

		s=e+1;
		e=line.find(";",s);
	}
}

static
string underscore( const string &s ){
	string t=s;
	size_t i;
	if( (i=t.find("."))!=string::npos ){
		t.replace( i,1,"_" );
	}
	if( (i=t.find("-"))!=string::npos ){
		t.replace( i,1,"_" );
	}
	return t;
}

void parseLibs( const string &rt ){
	std::string toolchain=home+"/bin/" BB_TRIPLE;

	ifstream rti( toolchain+"/runtime."+rt+".i" );
	if (!rti.is_open()) {
		cerr<<"Cannot find interface file for runtime."<<rt<<endl;
		exit( 1 );
	}

	string line;
	vector<string> modules;
	while( getline( rti,line ) ) {
		string i="ID: ",e="ENTRY: ",d="DEPS: ";
		if( line.find( i )==0 ){
			line=line.substr( i.size() );
			rtdl.id=line;
		}else if( line.find( e )==0 ){
			line=line.substr( e.size() );
			rtdl.entry=line;
		}else if( line.find( d )==0 ){
			line=line.substr( d.size() );
			parse_list( line,modules );
		}
	}

	for( string &id:modules ){
		BBModule module;

		ifstream iface( toolchain+"/cfg/"+id+".i" );
		if( !iface.is_open() ){
			cerr<<"Cannot find interface file for "<<id<<"."<<rt<<endl;
			exit( 1 );
		}

		while( getline( iface,line ) ) {
			string i="ID: ",m="MODULES: ",f="FUNC: ";
			if( line.find( i )==0 ){
				line=line.substr( i.size() );
				module.id=line.substr( 3,line.size()-3 );
				module.ident=underscore( id );
			}else if( line.find( m )==0 ){
				line=line.substr( m.size() );

				vector<string> deps;
				parse_list( line,deps );
				for( auto &dep:deps ){
					dep=dep.substr( 3,dep.size()-1 );
					module.deps.push_back( dep );
				}
			}else if( line.find( f )==0 ){
				line=line.substr( f.size() );

				size_t s=0,e=1;
				if( !isalnum(line[0]) && line[0]!='_' ){
					s=1;
				}
				while( isalnum(line[e]) || line[e]=='_' ) e++;

				BBSymbol sym;
				sym.ident=line.substr( s,e-s );

				rtdl.syms[tolower(sym.ident)]=module.id;

				module.syms.push_back( sym );
			}
		}

		rtdl.order.push_back( module.id );

		rtdl.modules[module.id]=module;
	}
}

const char *linkLibs( const string &rt ){
	// parseLibs( rt );

	if( const char *p=linkRuntime( rt ) ) return p;

	if( const char *p=linkUserLibs() ) return p;

	return 0;
}

void closeLibs(){

	delete runtimeEnviron;
	if( linkerLib ) linkerLib->deleteModule( runtimeModule );
	if( runtimeLib ) runtimeLib->shutdown();
	if( runtimeHMOD ) CLOSELIB( runtimeHMOD );

	runtimeEnviron=0;
	linkerLib=0;
	runtimeLib=0;
	runtimeHMOD=0;
}
