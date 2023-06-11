
#include <bb/stub/stub.h>
#include <bb/runtime/runtime.h>
#include <bb/blitz/blitz.h>

#include <iostream>
#include <cstdlib>
#include <cstring>

extern "C" {
	#include <luajit.h>
	#include <lauxlib.h>
	void blitzlua_loadlib( lua_State *L );
}

class StdioDebugger : public Debugger{
private:
	bool trace;
	string file;
	int row,col;
public:
	StdioDebugger( bool t ):trace(t){
	}

	void debugRun(){
	}
	void debugStop(){
	}
	void debugStmt( int srcpos,const char *f ){
		file=string(f);
		row=(srcpos>>16)&0xffff;
		col=srcpos&0xffff;
		if( trace ) cout<<file<<":"<<"["<<row+1<<":"<<col<<"]"<<endl;
	}
	void debugEnter( void *frame,void *env,const char *func ){
	}
	void debugLeave(){
	}
	void debugLog( const char *msg ){
    cout<<file<<":"<<"["<<row+1<<":"<<col<<"] "<<msg<<endl;
	}
	void debugMsg( const char *msg,bool serious ){
	}
	void debugSys( void *msg ){
	}
};

#ifndef BB_WINDOWS
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

#ifdef BB_DEBUG
#define BACKTRACE_DEPTH 10
static
void dump_backtrace(int sig) {
	void *array[BACKTRACE_DEPTH];

	size_t size=backtrace( array,BACKTRACE_DEPTH );

	fprintf(stderr, "Error (%d):\n", sig);
	backtrace_symbols_fd( array,size,STDERR_FILENO );

	exit( 1 );
}
#endif
#endif

extern "C" void bbMain();

int main( int argc,char *argv[] ){
#ifndef BB_WINDOWS
#ifdef BB_DEBUG
	signal(SIGSEGV, dump_backtrace);
	bb_env.debug=true;
#else
	bb_env.debug=false;
#endif
#endif

	bool trace=false;
	string cmd_line;
	for( int i=1;i<argc;i++ ){
		if( strncmp( "--trace",argv[i],strlen("--trace")+1 )==0 ){
			trace=true;
		}
		cmd_line+=argv[i];
	}
	bbStartup( argv[0],cmd_line.c_str() );

	StdioDebugger debugger( trace );
	bbAttachDebugger( &debugger );

	if( !(bbRuntime=bbCreateRuntime()) ){
	cerr<<"Failed to create runtime"<<endl;
	return 1;
	}

	int retcode=0;
	try{
		if( !bbruntime_create() ) return 1;

		char buff[256];
		int error;

		lua_State *L=luaL_newstate();

		blitzlua_loadlib( L );

		if( luaL_dofile( L,cmd_line.c_str() )!=LUA_OK ){
			cerr<<lua_tostring( L,-1 )<<endl;
			retcode=1;
		}

		lua_close( L );
	}catch( bbEx &ex ){
		if( ex.err ){
			if( bb_env.debug ){
				debugger.debugLog( ex.err );
			}else{
				cerr<<ex.err<<endl;
			}
		}
		retcode=1;
	}
	bbruntime_destroy();
	return retcode;
}
