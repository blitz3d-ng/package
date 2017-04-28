
#include <bb/stub/stub.h>
#include <bb/runtime/runtime.h>

#include <string>
using namespace std;

//start up error
void sue( const char *t ){
	string p=string( "Startup Error: " )+t;
	_bbDebugInfo( p.c_str() );
}

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
