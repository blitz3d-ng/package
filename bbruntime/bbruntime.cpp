
#include "std.h"
#include "bbsys.h"
#include "bbruntime.h"

#include "../gxruntime/gxruntime.h"
extern gxRuntime *gx_runtime;

void BBCALL bbEnd(){
	RTEX( 0 );
}
void BBCALL bbStop(){
	gx_runtime->debugStop();
	if( !gx_runtime->idle() ) RTEX( 0 );
}

void BBCALL bbAppTitle( BBStr *ti,BBStr *cp ){
	gx_runtime->setTitle( *ti,*cp );
	delete ti;delete cp;
}

void BBCALL bbRuntimeError( BBStr *str ){
	string t=*str;delete str;
	if( t.size()>255 ) t[255]=0;
	static char err[256];
	strcpy( err,t.c_str() );
	RTEX( err );
}

int BBCALL bbExecFile( BBStr *f ){
	string t=*f;delete f;
	int n=gx_runtime->execute( t );
	if( !gx_runtime->idle() ) RTEX( 0 );
	return n;
}

void BBCALL bbDelay( int ms ){
	if( !gx_runtime->delay( ms ) ) RTEX( 0 );
}

int BBCALL bbMilliSecs(){
	return gx_runtime->getMilliSecs();
}

BBStr * BBCALL bbCommandLine(){
	return d_new BBStr( gx_runtime->commandLine() );
}

BBStr * BBCALL bbSystemProperty( BBStr *p ){
	string t=gx_runtime->systemProperty( *p );
	delete p;return d_new BBStr( t );
}

BBStr * BBCALL bbGetEnv( BBStr *env_var ){
	char *p=getenv( env_var->c_str() );
	BBStr *val=d_new BBStr( p ? p : "" );
	delete env_var;
	return val;
}

void BBCALL bbSetEnv( BBStr *env_var,BBStr *val ){
	string t=*env_var+"="+*val;
	_putenv( t.c_str() );
	delete env_var;
	delete val;
}

gxTimer * BBCALL bbCreateTimer( int hertz ){
	gxTimer *t=gx_runtime->createTimer( hertz );
	return t;
}

int BBCALL bbWaitTimer( gxTimer *t ){
	int n=t->wait();
	if( !gx_runtime->idle() ) RTEX( 0 );
	return n;
}

void BBCALL bbFreeTimer( gxTimer *t ){
	gx_runtime->freeTimer( t );
}

void BBCALL bbDebugLog( BBStr *t ){
	gx_runtime->debugLog( t->c_str() );
	delete t;
}

void BBCALL _bbDebugStmt( int pos,const char *file ){
	gx_runtime->debugStmt( pos,file );
	if( !gx_runtime->idle() ) RTEX( 0 );
}

void BBCALL _bbDebugEnter( void *frame,void *env,const char *func ){
	gx_runtime->debugEnter( frame,env,func );
}

void BBCALL _bbDebugLeave(){
	gx_runtime->debugLeave();
}

BBMODULE_DECL( basic );
BBMODULE_DECL( math );
BBMODULE_DECL( string );
BBMODULE_DECL( system );
BBMODULE_DECL( stream );
BBMODULE_DECL( sockets );
BBMODULE_DECL( filesystem );
BBMODULE_DECL( bank );
BBMODULE_DECL( graphics );
BBMODULE_DECL( input );
BBMODULE_DECL( audio );
BBMODULE_DECL( multiplay );
BBMODULE_DECL( userlibs );
#ifdef PRO
BBMODULE_DECL( blitz3d );
#else
bool blitz3d_create(){ return true; }
bool blitz3d_destroy(){ return true; }
void blitz3d_link( void (*rtSym)( const char *sym,void *pc ) ){}
#endif

// TODO: fix this later
#undef rtSym
#define rtSym( sym,pc ) rtSym( sym,(void*)pc );

void bbruntime_link( void (*rtSym)( const char *sym,void *pc ) ){

	rtSym( "End",bbEnd );
	rtSym( "Stop",bbStop );
	rtSym( "AppTitle$title$close_prompt=\"\"",bbAppTitle );
	rtSym( "RuntimeError$message",bbRuntimeError );
	rtSym( "ExecFile$command",bbExecFile );
	rtSym( "Delay%millisecs",bbDelay );
	rtSym( "%MilliSecs",bbMilliSecs );
	rtSym( "$CommandLine",bbCommandLine );
	rtSym( "$SystemProperty$property",bbSystemProperty );
	rtSym( "$GetEnv$env_var",bbGetEnv );
	rtSym( "SetEnv$env_var$value",bbSetEnv );

	rtSym( "%CreateTimer%hertz",bbCreateTimer );
	rtSym( "%WaitTimer%timer",bbWaitTimer );
	rtSym( "FreeTimer%timer",bbFreeTimer );
	rtSym( "DebugLog$text",bbDebugLog );

	rtSym( "_bbDebugStmt",_bbDebugStmt );
	rtSym( "_bbDebugEnter",_bbDebugEnter );
	rtSym( "_bbDebugLeave",_bbDebugLeave );

	basic_link( rtSym );
	math_link( rtSym );
	string_link( rtSym );
	system_link( rtSym );
	stream_link( rtSym );
	sockets_link( rtSym );
	filesystem_link( rtSym );
	bank_link( rtSym );
	graphics_link( rtSym );
	input_link( rtSym );
	audio_link( rtSym );
	// multiplay_link( rtSym );
	blitz3d_link( rtSym );
	userlibs_link( rtSym );
}

//start up error
static void sue( const char *t ){
	string p=string( "Startup Error: " )+t;
	gx_runtime->debugInfo( p.c_str() );
}

bool bbruntime_create(){
	if( basic_create() ){
		if( math_create() ){
			if( string_create() ){
				if( system_create() ){
					if( stream_create() ){
						if( sockets_create() ){
							if( filesystem_create() ){
								if( bank_create() ){
									if( graphics_create() ){
										if( input_create() ){
											if( audio_create() ){
												// if( multiplay_create() ){
													if( blitz3d_create() ){
														if( userlibs_create() ){
															return true;
														}
													}else sue( "blitz3d_create failed" );
												// 	multiplay_destroy();
												// }else sue( "multiplay_create failed" );
												audio_destroy();
											}else sue( "audio_create failed" );
											input_destroy();
										}else sue( "input_create failed" );
										graphics_destroy();
									}else sue( "graphics_create failed" );
									bank_destroy();
								}else sue( "bank_create failed" );
								filesystem_destroy();
							}else sue( "filesystem_create failed" );
							sockets_destroy();
						}else sue( "sockets_create failed" );
						stream_destroy();
					}else sue( "stream_create failed" );
					system_destroy();
				}else sue( "system_create failed" );
				string_destroy();
			}else sue( "string_create failed" );
			math_destroy();
		}else sue( "math_create failed" );
		basic_destroy();
	}else sue( "basic_create failed" );
	return false;
}

bool bbruntime_destroy(){
	userlibs_destroy();
	blitz3d_destroy();
	// multiplay_destroy();
	audio_destroy();
	input_destroy();
	graphics_destroy();
	bank_destroy();
	filesystem_destroy();
	sockets_destroy();
	stream_destroy();
	string_destroy();
	math_destroy();
	basic_destroy();
	return true;
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
