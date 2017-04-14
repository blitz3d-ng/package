
#include <bb/runtime/runtime.h>
#include "debug.h"

#include "../../gxruntime/gxruntime.h"
extern gxRuntime *gx_runtime;

static Debugger *debugger;

void BBCALL bbAttachDebugger( Debugger *d ){
	debugger=d;
}

void BBCALL _bbDebugLog( const char *t ){
	if( debugger ) debugger->debugLog( t );
}

void BBCALL _bbDebugInfo( const char *e ){
  gx_runtime->debugInfo( e );
}

void BBCALL _bbDebugError( const char *e ){
  gx_runtime->debugError( e );
}

void BBCALL _bbDebugStmt( int pos,const char *file ){
	if( debugger ) debugger->debugStmt( pos,file );
	if( !bbRuntimeIdle() ) RTEX( 0 );
}

void BBCALL _bbDebugEnter( void *frame,void *env,const char *func ){
	if( debugger ) debugger->debugEnter( frame,env,func );
}

void BBCALL _bbDebugLeave(){
	if( debugger ) debugger->debugLeave();
}

void BBCALL _bbDebugStop(){
	gx_runtime->debugStop();
}

void BBCALL _bbDebugRun(){
	if( debugger ) debugger->debugRun();
}

void BBCALL bbDebugLog( BBStr *t ){
	_bbDebugLog( t->c_str() );
	delete t;
}
