#include "timer.h"

#include "../../gxruntime/gxruntime.h"
extern gxRuntime *gx_runtime;

BBTimer * BBCALL bbCreateTimer( int hertz ){
	BBTimer *t=gx_runtime->createTimer( hertz );
	return t;
}

int BBCALL bbWaitTimer( BBTimer *t ){
	int n=t->wait();
	if( !gx_runtime->idle() ) RTEX( 0 );
	return n;
}

void BBCALL bbFreeTimer( BBTimer *t ){
	gx_runtime->freeTimer( t );
}

BBMODULE_CREATE( timer ){
  return true;
}

BBMODULE_DESTROY( timer ){
  return true;
}

BBMODULE_LINK( timer ){
  rtSym( "%CreateTimer%hertz",bbCreateTimer );
  rtSym( "%WaitTimer%timer",bbWaitTimer );
  rtSym( "FreeTimer%timer",bbFreeTimer );
}
