
#include <bb/stub/stub.h>
#include <bb/runtime/runtime.h>

#include <string>

extern "C" int BBCALL bbStart( int argc,char *argv[], BBMAIN bbMain ) {
	std::string cmd_line;
	for( int i=1;i<argc;i++ ){
		cmd_line+=argv[i];
	}
	bbStartup( argv[0],cmd_line.c_str() );
	const char *t=bbruntime_run( bbMain,true );
	return t!=0;
};

const char *bbruntime_run( void (*pc)(),bool dbg ){
	bb_env.debug=dbg;

	if( !bbruntime_create() ) return "Unable to start program";
	const char *t=0;
	try{
		if( !bbRuntimeIdle() ) RTEX( 0 );
		pc();
		_bbDebugInfo( "Program has ended" );
	}catch( bbEx x ){
		t=x.err;
	}
	bbruntime_destroy();
	return t;
}

void bbruntime_panic( const char *err ){
	RTEX( err );
}
