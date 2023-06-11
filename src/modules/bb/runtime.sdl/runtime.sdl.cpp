#include "../stdutil/stdutil.h"
#include "runtime.sdl.h"
#include <bb/pixmap/pixmap.h>
#include <bb/event/event.h>
#include <bb/system/system.h>
#include <bb/input/input.h>
#include <bb/hook/hook.h>
#ifdef BB_NDK
#include <bb/system.ndk/system.ndk.h>
#endif

#include <SDL_syswm.h>

#include <map>

#include "scancodes.cpp"

#include <bb/input/input.h>

class SDLInputDriver : public BBInputDriver{
public:
	~SDLInputDriver(){}

	BBDevice *getJoystick( int port )const{ return 0; }
	int getJoystickType( int port )const{ return 0; }
	int numJoysticks()const{ return 0; }

	int toAscii( int key )const{
		if( key==28 ) return '\n';
		switch( key ){
		case 21: return 'y';
		case 37: return 'K';
		default:
			LOGD( "unmapped key: %i", key );
			return 0;
		}
	}
};

class SDLJoystick : public BBDevice{
private:
	SDL_Joystick *js;
public:
	SDLJoystick( SDL_Joystick *js ):js(js){
		memset( axis_states,0,sizeof(axis_states) );
		memset( down_state,0,sizeof(down_state) );

		SDL_JoystickGetGUIDString( SDL_JoystickGetGUID(js),id,sizeof(id) );
		snprintf( name,sizeof(name),"%s",SDL_JoystickName( js ) );
	}

	void update(){
		int ax_count=SDL_JoystickNumAxes( js );
		if( ax_count>32 ) ax_count=32;

		for( int i=0;i<ax_count;i++ ){
			axis_states[i]=(float)SDL_JoystickGetAxis( js,i )/SHRT_MAX;
		}

		int btn_count=SDL_JoystickNumButtons( js );
		for( int i=0;i<32;i++ ){
			setDownState( i,SDL_JoystickGetButton( js,i ) );
		}
	}
};

std::map<SDL_Window*,SDLRuntime*> runtimes;

BBRuntime *bbCreateOpenGLRuntime(){
	return d_new SDLRuntime();
}

SDLRuntime::SDLRuntime():graphics(0){
	// runtimes.insert( make_pair( wnd,this ) );
	// bbAppOnChange.add( _refreshTitle,this );
}

SDLRuntime::~SDLRuntime(){
	// bbAppOnChange.del( _refreshTitle,this );
	SDL_Quit();
}

void SDLRuntime::afterCreate(){
	SDL_InitSubSystem( SDL_INIT_JOYSTICK );

	gx_input=d_new SDLInputDriver();

	for( int i=0;i<SDL_NumJoysticks();i++ ){
		SDL_Joystick *js=SDL_JoystickOpen( i );
		if( js ){
			SDLJoystick *j=d_new SDLJoystick( js );
			bbJoysticks.push_back( j );
		}
	}

#ifdef BB_NDK
	NDKSystemDriver *sysdriver=(NDKSystemDriver*)bbSystemDriver;
	sysdriver->setJNI( (jobject)SDL_AndroidGetActivity(),(JNIEnv*)SDL_AndroidGetJNIEnv() );
#endif

	BBContextDriver::change( "gl" );
	bbDefaultGraphics();
}

void SDLRuntime::asyncStop(){
}

void SDLRuntime::asyncRun(){
}

void SDLRuntime::asyncEnd(){
}

bool SDLRuntime::idle(){
	SDL_Event event;
	while( SDL_PollEvent(&event) ){
		if( event.type == SDL_QUIT ){
			RTEX( 0 );
		}else if( event.type==SDL_WINDOWEVENT ){
			if( event.window.event==SDL_WINDOWEVENT_RESIZED ) {
			}
		}else if( event.type==SDL_MOUSEMOTION ){
			BBEvent ev( BBEVENT_MOUSEMOVE,0,event.motion.x,event.motion.y );
			bbOnEvent.run( &ev );
		}else if( event.type==SDL_MOUSEBUTTONDOWN||event.type==SDL_MOUSEBUTTONUP ){
			BBEvent ev( event.type==SDL_MOUSEBUTTONDOWN?BBEVENT_MOUSEDOWN:BBEVENT_MOUSEUP,event.button.button );
			bbOnEvent.run( &ev );
		}else if( (event.type==SDL_KEYDOWN||event.type==SDL_KEYUP) && event.key.repeat==0 ){
			int code=event.key.keysym.scancode;
			if( code>=MAX_SDL_SCANCODES ) continue;

			int key=SDL_SCANCODE_MAP[code];
			if( !key ){
				std::cout<<"unmapped key code: "<<code<<std::endl;
				continue;
			}

			BBEvent ev;
			switch( event.type ){
			case SDL_KEYDOWN:
				ev=BBEvent( BBEVENT_KEYDOWN,key );
				break;
			case SDL_KEYUP:
				ev=BBEvent( BBEVENT_KEYUP,key );
				break;
			default:
				continue;
			}
			bbOnEvent.run( &ev );
		}
	}

	return true;
}

void *SDLRuntime::window(){
	if( !graphics ) return 0;
#ifdef WIN32
	SDL_SysWMinfo info;
	SDL_VERSION( &info.version );
	SDL_GetWindowWMInfo( graphics->wnd,&info );
	return info.info.win.window;
#else
	return 0;
#endif
}

void SDLRuntime::moveMouse( int x,int y ){
	graphics->moveMouse( x,y );
}

void SDLRuntime::setPointerVisible( bool vis ){
	SDL_ShowCursor( vis?SDL_ENABLE:SDL_DISABLE );
}

void SDLRuntime::_refreshTitle( void *data,void *context ){
	((SDLRuntime*)context)->setTitle( ((BBApp*)data)->title.c_str() );
}

void SDLRuntime::setTitle( const char *title ){
	// SDL_SetWindowTitle( wnd,title );
}

BBMODULE_CREATE( runtime_sdl ){
	return true;
}

BBMODULE_DESTROY( runtime_sdl ){
	return true;
}
