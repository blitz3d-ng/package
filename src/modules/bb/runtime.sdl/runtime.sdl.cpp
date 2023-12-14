#include "../stdutil/stdutil.h"
#include "runtime.sdl.h"
#include <bb/pixmap/pixmap.h>
#include <bb/event/event.h>
#include <bb/system/system.h>
#include <bb/input/input.h>
#include <bb/hook/hook.h>
#ifdef BB_NDK
#include <bb/system/system.ndk.h>
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

	int toAscii( int code )const{
		return code&0xff;
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
		for( int i=0;i<btn_count;i++ ){
			setDownState( i,SDL_JoystickGetButton( js,i ) );
		}
	}
};

std::map<SDL_Window*,SDLRuntime*> runtimes;

BBRuntime *bbCreateOpenGLRuntime(){
	return d_new SDLRuntime();
}

SDLRuntime::SDLRuntime(){
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
	bbSetJNI( (jobject)SDL_AndroidGetActivity(),(JNIEnv*)SDL_AndroidGetJNIEnv() );
#endif

	BBContextDriver::change( "sdl" );
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
			int button=0;
			switch( event.button.button ){
			case SDL_BUTTON_LEFT:button=1;break;
			case SDL_BUTTON_MIDDLE:button=3;break;
			case SDL_BUTTON_RIGHT:button=2;break;
			}

			if( button ){
				BBEvent ev( event.type==SDL_MOUSEBUTTONDOWN?BBEVENT_MOUSEDOWN:BBEVENT_MOUSEUP,button );
				bbOnEvent.run( &ev );
			}
		}else if( (event.type==SDL_KEYDOWN||event.type==SDL_KEYUP) && event.key.repeat==0 ){
			int code=event.key.keysym.scancode;
			if( code>=MAX_SDL_SCANCODES ) continue;

			int key=SDL_SCANCODE_MAP[code];
			if( !key ){
				LOGD( "unmapped key code: %i",code );
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

			if( event.type==SDL_KEYDOWN ){
				BBEvent ev=BBEvent( BBEVENT_CHAR,0 );
				// LOGD( "code=%i",code );
				switch( code ){
				case SDL_SCANCODE_BACKSPACE:
					ev.data='\b';
					break;
				case SDL_SCANCODE_RETURN:case SDL_SCANCODE_RETURN2:case SDL_SCANCODE_KP_ENTER:
					ev.data='\n';
					break;
				case SDL_SCANCODE_UP:
					ev.data=BBInputDriver::ASC_UP;
					break;
				case SDL_SCANCODE_DOWN:
					ev.data=BBInputDriver::ASC_DOWN;
					break;
				case SDL_SCANCODE_LEFT:
					ev.data=BBInputDriver::ASC_LEFT;
					break;
				case SDL_SCANCODE_RIGHT:
					ev.data=BBInputDriver::ASC_RIGHT;
					break;
				}
				if( ev.data ) bbOnEvent.run( &ev );
			}
		}else if( event.type==SDL_TEXTINPUT||event.type==SDL_TEXTEDITING ){
			// LOGD( "text: %s",event.text.text );
			char *c=event.text.text;
			while( *c ){
				BBEvent ev=BBEvent( BBEVENT_CHAR,*(c++) );
				bbOnEvent.run( &ev );
			}
		}
	}

	return true;
}

void *SDLRuntime::window(){
	auto graphics=(SDLGraphics*)((SDLContextDriver*)bbContextDriver)->getGraphics();
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
	auto graphics=(SDLGraphics*)((SDLContextDriver*)bbContextDriver)->getGraphics();
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
