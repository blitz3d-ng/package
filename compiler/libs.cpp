
#include "libs.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <json.hpp>
using namespace nlohmann;

int bcc_ver;
int lnk_ver;
int run_ver;
int dbg_ver;

string home;
#ifdef WIN32
Linker *linkerLib;
Runtime *runtimeLib;

Module *runtimeModule;
#endif
Environ *runtimeEnviron;
vector<string> keyWords;
vector<UserFunc> userFuncs;

#ifdef WIN32
static HMODULE linkerHMOD,runtimeHMOD;
#endif

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

static const char *linkRuntime(){
#ifdef WIN32
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
		n=tolower(n);
		runtimeEnviron->funcDecls->insertDecl( n,f,DECL_FUNC );
		runtimeModule->addSymbol( ("_f"+n).c_str(),pc );
	}
#else
	json index;
	ifstream i("default.commands.json");
	i >> index;

	for( auto& command:index ) {
		string n=command["name"];
		bool cfunc=false;

		keyWords.push_back( n );

		string return_type=command["return_type"].is_string() ? command["return_type"] : "";

		Type *t;

		DeclSeq *params=d_new DeclSeq();
		for( auto& param:command["parameters"] ) {
			ConstType *defType=0;
			string ty=param["type"].is_string() ? param["type"] : "";
			t=_typeof( ty[0] );
			Decl *d=params->insertDecl( param["ident"],t,DECL_PARAM,defType );
		}

		t=_typeof( return_type[0] );
		FuncType *f=d_new FuncType( t,params,false,cfunc );
		n=tolower(n);
		runtimeEnviron->funcDecls->insertDecl( n,f,DECL_FUNC );
	}

#endif
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

const char *openLibs( string rt ){

	char *p=getenv( "blitzpath" );
	if( !p ) return "Can't find blitzpath environment variable";
#ifdef WIN32
	char buff[MAX_PATH];
	GetFullPathName( p,MAX_PATH,buff,NULL );
	home=string(buff);
#else
	home=string(p);
#endif

#ifdef WIN32
	linkerHMOD=LoadLibrary( (home+"/bin/linker.dll").c_str() );
	if( !linkerHMOD ) return "Unable to open linker.dll";

	typedef Linker *(_cdecl*GetLinker)();
	GetLinker gl=(GetLinker)GetProcAddress( linkerHMOD,"linkerGetLinker" );
	if( !gl ) return "Error in linker.dll";
	linkerLib=gl();

	runtimeHMOD=LoadLibrary( (home+"/bin/runtime."+rt+".dll").c_str() );
	if( !runtimeHMOD ) return ("Unable to open runtime."+rt+".dll").c_str();

	typedef Runtime *(_cdecl*GetRuntime)();
	GetRuntime gr=(GetRuntime)GetProcAddress( runtimeHMOD,"runtimeGetRuntime" );
	if( !gr ) return ("Error in runtime."+rt+".dll").c_str();
	runtimeLib=gr();
#endif

	bcc_ver=VERSION;
#ifdef WIN32
	lnk_ver=linkerLib->version();
	run_ver=runtimeLib->version();
#else
	lnk_ver=run_ver=bcc_ver;
#endif

	if( (lnk_ver>>16)!=(bcc_ver>>16) ||
		(run_ver>>16)!=(bcc_ver>>16) ||
		(lnk_ver>>16)!=(bcc_ver>>16) ) return "Library version error";

#ifdef WIN32
	runtimeLib->startup( GetModuleHandle(0) );

	runtimeModule=linkerLib->createModule();
#endif
	runtimeEnviron=d_new Environ( "",Type::int_type,0,0 );

	keyWords.clear();
	userFuncs.clear();

	return 0;
}

const char *linkLibs(){

	if( const char *p=linkRuntime() ) return p;

	if( const char *p=linkUserLibs() ) return p;

	return 0;
}

void closeLibs(){

	delete runtimeEnviron;
#ifdef WIN32
	if( linkerLib ) linkerLib->deleteModule( runtimeModule );
	if( runtimeLib ) runtimeLib->shutdown();
	if( runtimeHMOD ) FreeLibrary( runtimeHMOD );
	if( linkerHMOD ) FreeLibrary( linkerHMOD );
#endif

	runtimeEnviron=0;
#ifdef WIN32
	linkerLib=0;
	runtimeLib=0;
	runtimeHMOD=0;
	linkerHMOD=0;
#endif
}
