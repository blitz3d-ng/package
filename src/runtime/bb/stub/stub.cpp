
#include <bb/stub/stub.h>

#include "../../gxruntime/gxruntime.h"
extern gxRuntime *gx_runtime;

//start up error
void sue( const char *t ){
	string p=string( "Startup Error: " )+t;
	gx_runtime->debugInfo( p.c_str() );
}

const char *bbruntime_run( gxRuntime *rt,void (*pc)(),bool dbg ){
	bb_env.debug=dbg;
	gx_runtime=rt;

	if( !bbruntime_create() ) return "Unable to start program";
	const char *t=0;
	try{
		if( !gx_runtime->idle() ) RTEX( 0 );
		pc();
		gx_runtime->debugInfo( "Program has ended" );
	}catch( bbEx x ){
		t=x.err;
	}
	bbruntime_destroy();
	return t;
}

void bbruntime_panic( const char *err ){
	RTEX( err );
}
