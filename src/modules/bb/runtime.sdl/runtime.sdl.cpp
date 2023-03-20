

#include "../stdutil/stdutil.h"
#include "runtime.sdl.h"
#include <bb/pixmap/pixmap.h>
#include <bb/event/event.h>
#include <bb/system/system.h>
#include <bb/input/input.h>
#include <bb/hook/hook.h>

#include <SDL_syswm.h>

#include <map>
using namespace std;

#include "scancodes.cpp"

#include <bb/input/input.h>

class SDLInputDriver : public BBInputDriver{
public:
	~SDLInputDriver(){}

	BBDevice *getJoystick( int port )const{ return 0; }
	int getJoystickType( int port )const{ return 0; }
	int numJoysticks()const{ return 0; }

	int toAscii( int key )const{
		// cout<<key<<endl;
		if( key==28 ) return '\n';
		return 65;
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

map<SDL_Window*,SDLRuntime*> runtimes;

BBRuntime *bbCreateOpenGLRuntime(){
	if( SDL_Init(SDL_INIT_VIDEO)<0 ) return 0;

#ifdef BB_DESKTOP
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION,4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION,0 );
#else
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION,3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION,0 );
#endif

	SDL_Window* window=SDL_CreateWindow( "",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1,1,SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI );
	if( window==NULL ){
		SDL_Quit();
		return 0;
	}

	return d_new SDLRuntime( window );
}


SDLRuntime::SDLRuntime( SDL_Window *wnd ):wnd(wnd),graphics(0){
	bbContextDriver=this;
	bbSceneDriver=this;

	runtimes.insert( make_pair( wnd,this ) );

	bbAppOnChange.add( _refreshTitle,this );
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
}

int SDLRuntime::numGraphicsDrivers(){
	return SDL_GetNumVideoDisplays();
}

void SDLRuntime::graphicsDriverInfo( int driver,std::string *name,int *c ){
}

int SDLRuntime::numGraphicsModes( int driver ){
	return SDL_GetNumDisplayModes( driver );
}

void SDLRuntime::graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c ){
	SDL_DisplayMode dm;
	SDL_GetDisplayMode( driver,mode,&dm );

	*w=dm.w;
	*h=dm.h;
	*d=32; // TODO: https://wiki.libsdl.org/SDL_PixelFormatEnum
	*c=GFXMODECAPS_3D;
}

void SDLRuntime::windowedModeInfo( int *c ){
	*c=GFXMODECAPS_3D;
}

class SDLDefaultCanvas : public GLB2DDefaultCanvas{
protected:
	SDL_Window *wnd;
public:
	SDLDefaultCanvas( SDL_Window *wnd,unsigned framebuffer,int mode,int flags ):GLB2DDefaultCanvas(framebuffer,mode,flags),wnd(wnd){}

	// int getWidth()const{
	//   // int width,height;
	//   // glfwGetWindowSize( wnd,&width,&height );
	//   // return width;
	// }
	//
	// int getHeight()const{
	//   // int width,height;
	//   // glfwGetWindowSize( wnd,&width,&height );
	//   // return height;
	// }

	void getViewport( int *x,int *y,int *w,int *h )const{
		*x=0;*y=0;*w=getWidth();*h=getHeight();
	}
};

class SDLGraphics : public B2DGraphics{
protected:
	SDL_Window *wnd;
	int window_width,window_height,drawable_width,drawable_height;

	unsigned short gamma_red[256], gamma_green[256], gamma_blue[256];

	BBImageFont *def_font;
public:
	SDLGraphics( SDL_Window *wnd ):wnd(wnd){
		unsigned framebuffer=0;
#ifdef BB_IOS
		// ios doesn't supply a default framebuffer
		GL( glGetIntegerv( GL_FRAMEBUFFER_BINDING,(int*)&framebuffer ) );
		_bbLog( "framebuffer: %i\n", framebuffer );
#endif

		front_canvas=d_new SDLDefaultCanvas( wnd,framebuffer,GL_FRONT,0 );
		back_canvas=d_new SDLDefaultCanvas( wnd,framebuffer,GL_BACK,0 );

		def_font=(BBImageFont*)loadFont( "courier",12,0 );
		if( def_font==0 ){
			def_font=(BBImageFont*)loadFont( "courier new",12,0 );
		}

		for( int k=0;k<256;++k ) gamma_red[k]=gamma_green[k]=gamma_blue[k]=k;

		resize();
	}

	~SDLGraphics(){
		if( front_canvas ) delete front_canvas;
		if( back_canvas ) delete back_canvas;
		front_canvas=back_canvas=0;
	}

	void resize(){
		SDL_GetWindowSize( wnd,&window_width,&window_height );
		SDL_GL_GetDrawableSize( wnd,&drawable_width,&drawable_height );

		((GLB2DCanvas*)front_canvas)->resize( window_width,window_height,getDensity() );
		((GLB2DCanvas*)back_canvas)->resize( window_width,window_height,getDensity() );
	}

	// graphics
	void backup(){}
	bool restore(){ return true; }

	//MANIPULATORS
	void vwait(){}

	//SPECIAL!
	void copy( BBCanvas *dest,int dx,int dy,int dw,int dh,BBCanvas *src,int sx,int sy,int sw,int sh ){}

  //NEW! Gamma control!
	void setGamma( int r,int g,int b,float dr,float dg,float db ){
		gamma_red[r&255]=dr*257.0f;
		gamma_red[g&255]=dg*257.0f;
		gamma_red[b&255]=db*257.0f;
	}
	void getGamma( int r,int g,int b,float *dr,float *dg,float *db ){
		*dr=gamma_red[r&255];*dg=gamma_green[g&255];*db=gamma_blue[b&255];
	}

	void updateGamma( bool calibrate ){
		SDL_SetWindowGammaRamp( wnd,gamma_red,gamma_green,gamma_blue );
	}

	//ACCESSORS
	int getWidth()const{ return drawable_width; }
	int getHeight()const{ return drawable_height; }
	int getLogicalWidth()const{ return window_width; };
	int getLogicalHeight()const{ return window_height; };
	int getDepth()const{ return 0; }
	float getDensity()const{ return (float)drawable_width/window_width; };
	int getScanLine()const{ return 0; }
	int getAvailVidmem()const{ return 0; }
	int getTotalVidmem()const{ return 0; }

	BBFont *getDefaultFont()const{ return def_font; }

	//OBJECTS
	BBCanvas *createCanvas( int width,int height,int flags ){
		BBCanvas *canvas=d_new GLB2DTextureCanvas( width,height,flags );
		canvas_set.insert( canvas );
		return canvas;
	}

	BBCanvas *loadCanvas( const std::string &file,int flags ){
		BBPixmap *pixmap=bbLoadPixmap( file );
		if( !pixmap ) return 0;

		BBCanvas *canvas=d_new GLB2DTextureCanvas( pixmap,flags );
		canvas_set.insert( canvas );
		delete pixmap;
		return canvas;
	}

	// b2dgraphics
	BBMovie *openMovie( const std::string &file,int flags ){ return 0; }

	BBFont *loadFont( const std::string &font,int height,int flags ){
		BBFont *f=BBImageFont::load( font,height,flags );
		font_set.insert( f );
		return f;
	}

	void moveMouse( int x,int y ){
		SDL_WarpMouseInWindow( wnd,x,y );
	}

	void setPointerVisible( bool vis ){
		SDL_ShowCursor( vis?SDL_ENABLE:SDL_DISABLE );
	}
};

BBGraphics *SDLRuntime::openGraphics( int w,int h,int d,int driver,int flags ){
	if( graphics ) return 0;

	bool windowed=flags & BBGraphics::GRAPHICS_WINDOWED ? true : false;
	bool scaled=windowed && (flags & BBGraphics::GRAPHICS_SCALED ? true : false);

#ifndef BB_DESKTOP
	// no point in 'windowed' on mobile, right?
	windowed=false;
	scaled=false;
#else
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode( 0,&mode );
	SDL_SetWindowPosition( wnd,(mode.w-w)/2.0f,(mode.h-h)/2.0f );
	SDL_SetWindowSize( wnd,w,h );
	SDL_SetWindowResizable( wnd,scaled?SDL_TRUE:SDL_FALSE );
#endif
	SDL_ShowWindow( wnd );

	static bool inited=false;
	if( !inited ){
		if( !SDL_GL_CreateContext( wnd ) ){
			_bbLog( "%s",SDL_GetError() );
			return 0;
		}

		int screen_w,screen_h;
		int drawableW,drawableH;

		SDL_GetWindowSize( wnd,&screen_w,&screen_h );
		SDL_GL_GetDrawableSize( wnd,&drawableW,&drawableH );

		// _bbLog( "GL Version:  %s\n",glGetString( GL_VERSION ) );
		// _bbLog( "GL Vendor:   %s\n",glGetString( GL_VENDOR ) );
		// _bbLog( "GL window:   %ix%i\n",screen_w,screen_h );
		// _bbLog( "GL drawable: %ix%i\n",drawableW,drawableH );

		GL( glViewport( 0.0,0.0,drawableW,drawableH ) );
		GL( glScissor( 0.0,0.0,drawableW,drawableH ) );
		GL( glClearColor( 0.0,0.0,0.0,1.0 ) );
		GL( glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT ) );

		inited=true;
	}

#ifdef BB_DESKTOP
	static bool glewOk=false;
	if( !glewOk ) {
		glewInit();
		glewOk=true;
	}
#endif

	if( windowed ){
		SDL_SetWindowFullscreen( wnd,0 );
	} else {
		SDL_SetWindowFullscreen( wnd,SDL_WINDOW_FULLSCREEN );
	}

	if( (graphics=d_new SDLGraphics( wnd )) ){
		SDL_RaiseWindow( wnd );
		return graphics;
	}
	return 0;
}

void SDLRuntime::closeGraphics( BBGraphics *g ){
	if( graphics!=g || !g ) return;
	delete graphics;graphics=0;
	SDL_HideWindow( wnd );
}

bool SDLRuntime::graphicsLost(){
	return true;
}

void SDLRuntime::flip( bool vwait ){
	// SDL_GL_SetSwapInterval( vwait ? 1 : 0 );
	SDL_GL_SwapWindow( wnd );
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
				resize();
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
				cout<<"unmapped key code: "<<code<<endl;
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
#ifdef WIN32
	SDL_SysWMinfo info;
	SDL_VERSION( &info.version );
	SDL_GetWindowWMInfo( wnd,&info );
	return info.info.win.window;
#else
	return 0;
#endif
}

void SDLRuntime::moveMouse( int x,int y ){
	graphics->moveMouse( x,y );
}

void SDLRuntime::setPointerVisible( bool vis ){
	graphics->setPointerVisible( vis );
}

void SDLRuntime::_refreshTitle( void *data,void *context ){
	((SDLRuntime*)context)->setTitle( ((BBApp*)data)->title.c_str() );
}

void SDLRuntime::setTitle( const char *title ){
	SDL_SetWindowTitle( wnd,title );
}

void SDLRuntime::resize(){
	if( graphics ) {
		graphics->resize();
		GLB3DGraphics::resize( graphics->getLogicalWidth(),graphics->getLogicalHeight(),graphics->getDensity() );
	}
}

BBMODULE_EMPTY( runtime_sdl );
