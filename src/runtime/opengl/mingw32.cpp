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
BBMODULE_DECL( stdio );
BBMODULE_DECL( stream );
BBMODULE_DECL( sockets );
BBMODULE_DECL( enet );
BBMODULE_DECL( runtime );
BBMODULE_DECL( system );
BBMODULE_DECL( bank );
BBMODULE_DECL( system_windows );
BBMODULE_DECL( filesystem );
BBMODULE_DECL( filesystem_windows );
BBMODULE_DECL( timer );
BBMODULE_DECL( timer_windows );
BBMODULE_DECL( audio );
BBMODULE_DECL( audio_fmod );
BBMODULE_DECL( userlibs );
BBMODULE_DECL( input );
BBMODULE_DECL( pixmap );
BBMODULE_DECL( blitz2d );
BBMODULE_DECL( graphics );
BBMODULE_DECL( graphics_gl );
BBMODULE_DECL( blitz3d );
BBMODULE_DECL( blitz2d_gl );
BBMODULE_DECL( blitz3d_gl );
BBMODULE_DECL( runtime_glfw3 );

class BBRuntime;
BBRuntime *bbCreateOpenGLRuntime();
BBRuntime *bbCreateRuntime(){
	return bbCreateOpenGLRuntime();
}


void bbruntime_link( void (*link)( const char *sym,void *pc ) ){
	blitz_link( link );
	math_link( link );
	string_link( link );
	stdio_link( link );
	stream_link( link );
	sockets_link( link );
	enet_link( link );
	runtime_link( link );
	system_link( link );
	bank_link( link );
	system_windows_link( link );
	filesystem_link( link );
	timer_link( link );
	audio_link( link );
	userlibs_link( link );
	input_link( link );
	blitz2d_link( link );
	graphics_link( link );
	blitz3d_link( link );
}

bool bbruntime_create(){
	if( blitz_create() ){
				if( hook_create() ){
						if( event_create() ){
								if( math_create() ){
										if( string_create() ){
												if( stdio_create() ){
														if( stream_create() ){
																if( sockets_create() ){
																		if( enet_create() ){
																				if( runtime_create() ){
																						if( system_create() ){
																								if( bank_create() ){
																										if( system_windows_create() ){
																												if( filesystem_create() ){
																														if( filesystem_windows_create() ){
																																if( timer_create() ){
																																		if( timer_windows_create() ){
																																				if( audio_create() ){
																																						if( audio_fmod_create() ){
																																								if( userlibs_create() ){
																																										if( input_create() ){
																																												if( pixmap_create() ){
																																														if( blitz2d_create() ){
																																																if( graphics_create() ){
																																																		if( graphics_gl_create() ){
																																																				if( blitz3d_create() ){
																																																						if( blitz2d_gl_create() ){
																																																								if( blitz3d_gl_create() ){
																																																										if( runtime_glfw3_create() ){
																																																												return true;
																													}else sue( "runtime_glfw3_create failed" );bbStart( 0,0,0 );
																													blitz3d_gl_destroy();
																												}else sue( "blitz3d_gl_create failed" );bbStart( 0,0,0 );
																												blitz2d_gl_destroy();
																											}else sue( "blitz2d_gl_create failed" );bbStart( 0,0,0 );
																											blitz3d_destroy();
																										}else sue( "blitz3d_create failed" );bbStart( 0,0,0 );
																										graphics_gl_destroy();
																									}else sue( "graphics_gl_create failed" );bbStart( 0,0,0 );
																									graphics_destroy();
																								}else sue( "graphics_create failed" );bbStart( 0,0,0 );
																								blitz2d_destroy();
																							}else sue( "blitz2d_create failed" );bbStart( 0,0,0 );
																							pixmap_destroy();
																						}else sue( "pixmap_create failed" );bbStart( 0,0,0 );
																						input_destroy();
																					}else sue( "input_create failed" );bbStart( 0,0,0 );
																					userlibs_destroy();
																				}else sue( "userlibs_create failed" );bbStart( 0,0,0 );
																				audio_fmod_destroy();
																			}else sue( "audio_fmod_create failed" );bbStart( 0,0,0 );
																			audio_destroy();
																		}else sue( "audio_create failed" );bbStart( 0,0,0 );
																		timer_windows_destroy();
																	}else sue( "timer_windows_create failed" );bbStart( 0,0,0 );
																	timer_destroy();
																}else sue( "timer_create failed" );bbStart( 0,0,0 );
																filesystem_windows_destroy();
															}else sue( "filesystem_windows_create failed" );bbStart( 0,0,0 );
															filesystem_destroy();
														}else sue( "filesystem_create failed" );bbStart( 0,0,0 );
														system_windows_destroy();
													}else sue( "system_windows_create failed" );bbStart( 0,0,0 );
													bank_destroy();
												}else sue( "bank_create failed" );bbStart( 0,0,0 );
												system_destroy();
											}else sue( "system_create failed" );bbStart( 0,0,0 );
											runtime_destroy();
										}else sue( "runtime_create failed" );bbStart( 0,0,0 );
										enet_destroy();
									}else sue( "enet_create failed" );bbStart( 0,0,0 );
									sockets_destroy();
								}else sue( "sockets_create failed" );bbStart( 0,0,0 );
								stream_destroy();
							}else sue( "stream_create failed" );bbStart( 0,0,0 );
							stdio_destroy();
						}else sue( "stdio_create failed" );bbStart( 0,0,0 );
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
	runtime_glfw3_destroy();
	blitz3d_gl_destroy();
	blitz2d_gl_destroy();
	blitz3d_destroy();
	graphics_gl_destroy();
	graphics_destroy();
	blitz2d_destroy();
	pixmap_destroy();
	input_destroy();
	userlibs_destroy();
	audio_fmod_destroy();
	audio_destroy();
	timer_windows_destroy();
	timer_destroy();
	filesystem_windows_destroy();
	filesystem_destroy();
	system_windows_destroy();
	bank_destroy();
	system_destroy();
	runtime_destroy();
	enet_destroy();
	sockets_destroy();
	stream_destroy();
	stdio_destroy();
	string_destroy();
	math_destroy();
	event_destroy();
	hook_destroy();
	blitz_destroy();
	return true;
}
