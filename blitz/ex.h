#ifndef BLITZ_EX_H
#define BLITZ_EX_H

#include "debug.h"

struct bbEx{
	const char *err;
	bbEx( const char *e ):err(e){
		if( e ) bbDebugError( e );
	}
};

#define RTEX( _X_ ) throw bbEx( _X_ );

#endif
