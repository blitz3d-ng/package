
#include <bb/runtime/runtime.h>
#include "debug.h"

#include "../../gxruntime/gxruntime.h"
extern gxRuntime *gx_runtime;

void BBCALL _bbDebugLog( const char *t ){
	gx_runtime->debugLog( t );
}

void BBCALL _bbDebugInfo( const char *e ){
  gx_runtime->debugInfo( e );
}

void BBCALL _bbDebugError( const char *e ){
  gx_runtime->debugError( e );
}

void BBCALL _bbDebugStmt( int pos,const char *file ){
	gx_runtime->debugStmt( pos,file );
	if( !bbRuntimeIdle() ) RTEX( 0 );
}

void BBCALL _bbDebugEnter( void *frame,void *env,const char *func ){
	gx_runtime->debugEnter( frame,env,func );
}

void BBCALL _bbDebugLeave(){
	gx_runtime->debugLeave();
}

void BBCALL bbDebugStop(){
	gx_runtime->debugStop();
	if( !bbRuntimeIdle() ) RTEX( 0 );
}

void BBCALL bbDebugLog( BBStr *t ){
	_bbDebugLog( t->c_str() );
	delete t;
}
