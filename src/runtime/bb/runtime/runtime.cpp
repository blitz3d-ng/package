
#include "../../stdutil/stdutil.h"
#include <bb/blitz/blitz.h>
#include <bb/event/event.h>
#include "runtime.h"

BBRuntime *bbRuntime;
BBHook *bbRuntimeOnSuspend;
BBHook *bbRuntimeOnResume;

BBRuntime::~BBRuntime(){
}

void *bbRuntimeWindow(){
	return bbRuntime->window();
}

bool bbRuntimeIdle(){
	return bbRuntime->idle();
}

void BBCALL bbMoveMouse( int x,int y ){
  bbRuntime->moveMouse( x,y );
	BBEvent ev=BBEvent( BBEVENT_MOUSEMOVE,1,x,y );
	bbOnEvent.run( &ev );
}

void BBCALL bbShowPointer(){
	bbRuntime->setPointerVisible( true );
}

void BBCALL bbHidePointer(){
	bbRuntime->setPointerVisible( false );
}

BBMODULE_CREATE( runtime ){
  bbRuntimeOnSuspend=d_new BBHook();
  bbRuntimeOnResume=d_new BBHook();
  return true;
}

BBMODULE_DESTROY( runtime ){
  delete bbRuntimeOnSuspend;
  delete bbRuntimeOnResume;
  return true;
}
