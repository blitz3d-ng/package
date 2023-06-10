#include "timer.h"
#include <bb/runtime/runtime.h>
#include <set>

static std::set<BBTimer*> timers;

BBTimer::~BBTimer(){
}

BBTimer * BBCALL bbCreateTimer( bb_int_t hertz ){
	BBTimer *t=_bbCreateTimer( hertz );
	timers.insert( t );
	return t;
}

bb_int_t BBCALL bbWaitTimer( BBTimer *t ){
	int n=t->wait();
	if( !bbRuntimeIdle() ) RTEX( 0 );
	return n;
}

void BBCALL bbFreeTimer( BBTimer *t ){
	if( !timers.count( t ) ) return;
	timers.erase( t );
	delete t;
}

BBMODULE_CREATE( timer ){
  return true;
}

BBMODULE_DESTROY( timer ){
	while( timers.size() ) bbFreeTimer( *timers.begin() );
  return true;
}
