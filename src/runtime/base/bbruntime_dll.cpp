/* Win32 runtime dynamic link lib */

#pragma warning( disable:4786 )

#include "bbruntime_dll.h"
#include <bb/blitz/blitz.h>
#include <bb/runtime/runtime.h>

#include <map>
#include <eh.h>
#include <float.h>

#include <bb/stub/stub.h>

#include "resource.h"

class DummyDebugger : public Debugger{
public:
	virtual void debugRun(){}
	virtual void debugStop(){}// bbruntime_panic(0); }
	virtual void debugStmt( int srcpos,const char *file ){}
	virtual void debugEnter( void *frame,void *env,const char *func ){}
	virtual void debugLeave(){}
	virtual void debugLog( const char *msg ){}
	virtual void debugMsg( const char *e,bool serious ){
		if( serious ) MessageBox( 0,e,"Error!",MB_OK|MB_TOPMOST|MB_SETFOREGROUND );
	}
	virtual void debugSys( void *msg ){}
};

#ifdef PRODEMO
static void killer(){
	ExitProcess( -1 );
}
#endif

static void __cdecl seTranslator( unsigned int u,EXCEPTION_POINTERS* pExp ){
	switch( u ){
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		bbruntime_panic( "Integer divide by zero" );
	case EXCEPTION_ACCESS_VIOLATION:
		bbruntime_panic( "Memory access violation" );
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		bbruntime_panic( "Illegal instruction" );
	case EXCEPTION_STACK_OVERFLOW:
		bbruntime_panic( "Stack overflow!" );
	}
	bbruntime_panic( "Unknown runtime exception" );
}

int Runtime::execute( void (*pc)(),const char *args,Debugger *dbg ){

	bool debug=!!dbg;

	static DummyDebugger dummydebug;

	if( !dbg ) dbg=&dummydebug;

	trackmem( true );

#ifndef __MINGW32__
	_se_translator_function old_trans=_set_se_translator( seTranslator );
	_control87( _RC_NEAR|_PC_24|_EM_INVALID|_EM_ZERODIVIDE|_EM_OVERFLOW|_EM_UNDERFLOW|_EM_INEXACT|_EM_DENORMAL,0xfffff );
#endif

	bbAttachDebugger( dbg );

	//strip spaces from ends of args...
	std::string params=args;
	while( params.size() && params[0]==' ' ) params=params.substr( 1 );
	while( params.size() && params[params.size()-1]==' ' ) params=params.substr( 0,params.size()-1 );

	bbStartup( "a",params.c_str() );

	const char *t=bbruntime_run( pc,debug );

#ifndef __MINGW32__
	_control87( _CW_DEFAULT,0xfffff );
	_set_se_translator( old_trans );
#endif

	return t!=0;
}

/********************** BUTT UGLY DLL->EXE HOOK! *************************/

static void *module_pc;
static std::map<std::string,int> module_syms;
static std::map<std::string,int> runtime_syms;
static Runtime *runtime;

static void fail(){
	MessageBox( 0,"Unable to run Blitz Basic module",0,0 );
	ExitProcess(-1);
}

struct Sym{
	std::string name;
	int value;
};

static Sym getSym( void **p ){
	Sym sym;
	char *t=(char*)*p;
	while( char c=*t++ ) sym.name+=c;
	sym.value=*(int*)t+(bb_int_t)module_pc;
	*p=t+4;return sym;
}

static int findSym( const std::string &t ){
	std::map<std::string,int>::iterator it;

	it=module_syms.find( t );
	if( it!=module_syms.end() ) return it->second;
	it=runtime_syms.find( t );
	if( it!=runtime_syms.end() ) return it->second;

	std::string err="Can't find symbol: "+t;
	MessageBox( 0,err.c_str(),0,0 );
	ExitProcess(0);
	return 0;
}

static void link(){

	while( const char *sc=runtime->nextSym() ){

		std::string t(sc);

		if( t[0]=='_' ){
			runtime_syms["_"+t]=runtime->symValue(sc);
			continue;
		}

		if( t[0]=='!' ) t=t.substr(1);

		if( !isalnum(t[0]) ) t=t.substr(1);

		for( unsigned int k=0;k<t.size();++k ){
			if( isalnum(t[k]) || t[k]=='_' ) continue;
			t=t.substr( 0,k );break;
		}

		runtime_syms["_f"+tolower(t)]=runtime->symValue(sc);
	}

	HRSRC hres=FindResource( 0,MAKEINTRESOURCE(IDR_BBMODULE),RT_RCDATA );if( !hres ) fail();
	HGLOBAL hglo=LoadResource( 0,hres );if( !hglo ) fail();
	void *p=LockResource( hglo );if( !p ) fail();

	int sz=*(int*)p;p=(int*)p+1;

	//replace malloc for service pack 2 Data Execution Prevention (DEP).
	module_pc=VirtualAlloc( 0,sz,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE );

	memcpy( module_pc,p,sz );
	p=(char*)p+sz;

	int k,cnt;

	cnt=*(int*)p;p=(int*)p+1;
	for( k=0;k<cnt;++k ){
		Sym sym=getSym( &p );
		if( sym.value<(bb_int_t)module_pc || sym.value>=(bb_int_t)module_pc+sz ) fail();
		module_syms[sym.name]=sym.value;
	}

	cnt=*(int*)p;p=(int*)p+1;
	for( k=0;k<cnt;++k ){
		Sym sym=getSym( &p );
		int *pp=(int*)sym.value;
		int dest=findSym( sym.name );
		*pp+=dest-(bb_int_t)pp;
	}

	cnt=*(int*)p;p=(int*)p+1;
	for( k=0;k<cnt;++k ){
		Sym sym=getSym( &p );
		int *pp=(int*)sym.value;
		int dest=findSym( sym.name );
		*pp+=dest;
	}

	runtime_syms.clear();
	module_syms.clear();
}

#ifdef BB_MINGW
#define BBWINMAIN         _bbWinMain
#define DLLMAINCRTSTARTUP DllMainCRTStartup
#else
#define BBWINMAIN         bbWinMain
#define DLLMAINCRTSTARTUP _DllMainCRTStartup
#endif

extern "C" __declspec(dllexport) int __stdcall BBWINMAIN();
extern "C" BOOL __stdcall DLLMAINCRTSTARTUP( HANDLE,DWORD,LPVOID );

bool WINAPI DllMain( HANDLE module,DWORD reason,void *reserved ){
	return TRUE;
}

int __stdcall BBWINMAIN(){

	HINSTANCE inst=GetModuleHandle( 0 );

	DLLMAINCRTSTARTUP( inst,DLL_PROCESS_ATTACH,0 );

#ifdef BETA
	int ver=VERSION & 0x7fff;
	std::string t="Created with Blitz3D Beta V"+itoa( ver/100 )+"."+itoa( ver%100 );
	MessageBox( GetDesktopWindow(),t.c_str(),"Blitz3D Message",MB_OK );
#endif

#ifdef SCHOOLS
	MessageBox( GetDesktopWindow(),"Created with the schools version of Blitz Basic","Blitz Basic Message",MB_OK );
#endif

	runtime=runtimeGetRuntime();
	runtime->startup();

	link();

	//get cmd_line and params
	std::string cmd=GetCommandLine(),params;
	while( cmd.size() && cmd[0]==' ' ) cmd=cmd.substr( 1 );
	if( cmd.find( '\"' )==0 ){
		int n=cmd.find( '\"',1 );
		if( n!=std::string::npos ){
			params=cmd.substr( n+1 );
			cmd=cmd.substr( 1,n-1 );
		}
	}else{
		int n=cmd.find( ' ' );
		if( n!=std::string::npos ){
			params=cmd.substr( n+1 );
			cmd=cmd.substr( 0,n );
		}
	}

	runtime->execute( (void(*)())module_pc,params.c_str(),0 );
	runtime->shutdown();

	DLLMAINCRTSTARTUP( inst,DLL_PROCESS_DETACH,0 );

	ExitProcess(0);
	return 0;
}
