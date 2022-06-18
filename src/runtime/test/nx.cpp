// AUTOGENERATED. DO NOT EDIT.
// RUN `bin/blitz3d config` TO UPDATE.

#include <bb/stub/stub.h>


#include "../../stdutil/stdutil.h"


// TODO: get rid of the dirty bbStart hack that's needed to keep clang from stripping...
#ifdef BB_WINDOWS
#define bbStart( a,b,c ) ;
#endif

BBMODULE_DECL( blitz );
BBMODULE_DECL( hook );
BBMODULE_DECL( event );
BBMODULE_DECL( math );
BBMODULE_DECL( string );
BBMODULE_DECL( stream );
BBMODULE_DECL( bank );
BBMODULE_DECL( stdio );
BBMODULE_DECL( runtime );
BBMODULE_DECL( system );
BBMODULE_DECL( input );
BBMODULE_DECL( audio );
BBMODULE_DECL( unit_test );

class BBRuntime;
BBRuntime *bbCreateConsoleRuntime();
BBRuntime *bbCreateRuntime(){
	return bbCreateConsoleRuntime();
}

#ifdef WIN32
void bbruntime_link( void (*link)( const char *sym,void *pc ) ){
	blitz_link( link );
	math_link( link );
	string_link( link );
	stream_link( link );
	bank_link( link );
	stdio_link( link );
	runtime_link( link );
	system_link( link );
	input_link( link );
	audio_link( link );
	unit_test_link( link );
}
#endif
bool bbruntime_create(){
	if( blitz_create() ){
				if( hook_create() ){
						if( event_create() ){
								if( math_create() ){
										if( string_create() ){
												if( stream_create() ){
														if( bank_create() ){
																if( stdio_create() ){
																		if( runtime_create() ){
																				if( system_create() ){
																						if( input_create() ){
																								if( audio_create() ){
																										if( unit_test_create() ){
																												return true;
													}else sue( "unit_test_create failed" );bbStart( 0,0,0 );
													audio_destroy();
												}else sue( "audio_create failed" );bbStart( 0,0,0 );
												input_destroy();
											}else sue( "input_create failed" );bbStart( 0,0,0 );
											system_destroy();
										}else sue( "system_create failed" );bbStart( 0,0,0 );
										runtime_destroy();
									}else sue( "runtime_create failed" );bbStart( 0,0,0 );
									stdio_destroy();
								}else sue( "stdio_create failed" );bbStart( 0,0,0 );
								bank_destroy();
							}else sue( "bank_create failed" );bbStart( 0,0,0 );
							stream_destroy();
						}else sue( "stream_create failed" );bbStart( 0,0,0 );
						string_destroy();
					}else sue( "string_create failed" );bbStart( 0,0,0 );
					math_destroy();
				}else sue( "math_create failed" );bbStart( 0,0,0 );
				event_destroy();
			}else sue( "event_create failed" );bbStart( 0,0,0 );
			hook_destroy();
		}else sue( "hook_create failed" );bbStart( 0,0,0 );
		blitz_destroy();
	}else sue( "blitz_create failed" );bbStart( 0,0,0 );
	return false;
}

bool bbruntime_destroy(){
	unit_test_destroy();
	audio_destroy();
	input_destroy();
	system_destroy();
	runtime_destroy();
	stdio_destroy();
	bank_destroy();
	stream_destroy();
	string_destroy();
	math_destroy();
	event_destroy();
	hook_destroy();
	blitz_destroy();
	return true;
}
