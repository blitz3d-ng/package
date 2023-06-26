#include <bb/blitz/blitz.h>
#include <bb/runtime/runtime.h>
#include "stub.h"

bool bbruntime_run( void (*pc)(),bool dbg ){
	bb_env.debug=dbg;

	if( !bbruntime_create() ) return false; // "Unable to start program";
	bool err=false;
	try{
		if( !bbRuntimeIdle() ) RTEX( 0 );
		pc();
		_bbDebugInfo( "Program has ended" );
	}catch( bbEx &x ){
		if( x.err!="" ) {
			_bbDebugError( x.err.c_str() );
			err=true;
		}
	}
	bbruntime_destroy();
	return !err;
}

BBMODULE_CREATE( stub ){
	return true;
}

BBMODULE_DESTROY( stub ){
	return true;
}
