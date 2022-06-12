// AUTOGENERATED. DO NOT EDIT.
// RUN `bin/blitz3d config` TO UPDATE.

#include <bb/stub/stub.h>


#include "../../stdutil/stdutil.h"


// TODO: get rid of the dirty bbStart hack that's needed to keep clang from stripping...

BBMODULE_DECL( math );
BBMODULE_DECL( blitz );
BBMODULE_DECL( string );
BBMODULE_DECL( stream );
BBMODULE_DECL( bank );
BBMODULE_DECL( stdio );
BBMODULE_DECL( sockets );
BBMODULE_DECL( hook );
BBMODULE_DECL( enet );
BBMODULE_DECL( runtime );
BBMODULE_DECL( system );
BBMODULE_DECL( system_macos );
BBMODULE_DECL( filesystem );
BBMODULE_DECL( filesystem_posix );
BBMODULE_DECL( timer );
BBMODULE_DECL( timer_noop );
BBMODULE_DECL( input );
BBMODULE_DECL( audio );
BBMODULE_DECL( audio_openal );
BBMODULE_DECL( event );
BBMODULE_DECL( runtime_console );
BBMODULE_DECL( unit_test );

class BBRuntime;
BBRuntime *bbCreateConsoleRuntime();
BBRuntime *bbCreateRuntime(){
	return bbCreateConsoleRuntime();
}

#ifdef WIN32
void bbruntime_link( void (*link)( const char *sym,void *pc ) ){
	math_link( link );
	blitz_link( link );
	string_link( link );
	stream_link( link );
	bank_link( link );
	stdio_link( link );
	sockets_link( link );
	enet_link( link );
	runtime_link( link );
	system_link( link );
	filesystem_link( link );
	timer_link( link );
	input_link( link );
	audio_link( link );
	unit_test_link( link );
}
#endif
bool bbruntime_create(){
	if( math_create() ){
				if( blitz_create() ){
						if( string_create() ){
								if( stream_create() ){
										if( bank_create() ){
												if( stdio_create() ){
														if( sockets_create() ){
																if( hook_create() ){
																		if( enet_create() ){
																				if( runtime_create() ){
																						if( system_create() ){
																								if( system_macos_create() ){
																										if( filesystem_create() ){
																												if( filesystem_posix_create() ){
																														if( timer_create() ){
																																if( timer_noop_create() ){
																																		if( input_create() ){
																																				if( audio_create() ){
																																						if( audio_openal_create() ){
																																								if( event_create() ){
																																										if( runtime_console_create() ){
																																												if( unit_test_create() ){
																																														return true;
																						}else sue( "unit_test_create failed" );bbStart( 0,0,0 );
																						runtime_console_destroy();
																					}else sue( "runtime_console_create failed" );bbStart( 0,0,0 );
																					event_destroy();
																				}else sue( "event_create failed" );bbStart( 0,0,0 );
																				audio_openal_destroy();
																			}else sue( "audio_openal_create failed" );bbStart( 0,0,0 );
																			audio_destroy();
																		}else sue( "audio_create failed" );bbStart( 0,0,0 );
																		input_destroy();
																	}else sue( "input_create failed" );bbStart( 0,0,0 );
																	timer_noop_destroy();
																}else sue( "timer_noop_create failed" );bbStart( 0,0,0 );
																timer_destroy();
															}else sue( "timer_create failed" );bbStart( 0,0,0 );
															filesystem_posix_destroy();
														}else sue( "filesystem_posix_create failed" );bbStart( 0,0,0 );
														filesystem_destroy();
													}else sue( "filesystem_create failed" );bbStart( 0,0,0 );
													system_macos_destroy();
												}else sue( "system_macos_create failed" );bbStart( 0,0,0 );
												system_destroy();
											}else sue( "system_create failed" );bbStart( 0,0,0 );
											runtime_destroy();
										}else sue( "runtime_create failed" );bbStart( 0,0,0 );
										enet_destroy();
									}else sue( "enet_create failed" );bbStart( 0,0,0 );
									hook_destroy();
								}else sue( "hook_create failed" );bbStart( 0,0,0 );
								sockets_destroy();
							}else sue( "sockets_create failed" );bbStart( 0,0,0 );
							stdio_destroy();
						}else sue( "stdio_create failed" );bbStart( 0,0,0 );
						bank_destroy();
					}else sue( "bank_create failed" );bbStart( 0,0,0 );
					stream_destroy();
				}else sue( "stream_create failed" );bbStart( 0,0,0 );
				string_destroy();
			}else sue( "string_create failed" );bbStart( 0,0,0 );
			blitz_destroy();
		}else sue( "blitz_create failed" );bbStart( 0,0,0 );
		math_destroy();
	}else sue( "math_create failed" );bbStart( 0,0,0 );
	return false;
}

bool bbruntime_destroy(){
	unit_test_destroy();
	runtime_console_destroy();
	event_destroy();
	audio_openal_destroy();
	audio_destroy();
	input_destroy();
	timer_noop_destroy();
	timer_destroy();
	filesystem_posix_destroy();
	filesystem_destroy();
	system_macos_destroy();
	system_destroy();
	runtime_destroy();
	enet_destroy();
	hook_destroy();
	sockets_destroy();
	stdio_destroy();
	bank_destroy();
	stream_destroy();
	string_destroy();
	blitz_destroy();
	math_destroy();
	return true;
}
