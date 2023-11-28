#include "context.egl.h"

#include "egl.h"

#ifdef BB_NX
#include <switch.h>
#endif

class EGLGraphics : public GLGraphics{
protected:
	int window_width,window_height,drawable_width,drawable_height;

	unsigned short gamma_red[256], gamma_green[256], gamma_blue[256];

	static void onAppChange( void *data,void *context );
public:
	EGLGraphics( int w,int h );
	~EGLGraphics();

	// graphics
	void backup();
	bool restore();

	//MANIPULATORS
	void vwait();

	//SPECIAL!
	void copy( BBCanvas *dest,int dx,int dy,int dw,int dh,BBCanvas *src,int sx,int sy,int sw,int sh );

  //NEW! Gamma control!
	void setGamma( int r,int g,int b,float dr,float dg,float db );
	void getGamma( int r,int g,int b,float *dr,float *dg,float *db );
	void updateGamma( bool calibrate );

	//ACCESSORS
	int getWidth()const;
	int getHeight()const;
	int getLogicalWidth()const;
	int getLogicalHeight()const;
	int getDepth()const;
	float getDensity()const;
	int getScanLine()const;
	int getAvailVidmem()const;
	int getTotalVidmem()const;

	void moveMouse( int x,int y );
};

void EGLGraphics::onAppChange( void *data,void *context ){
	// EGLGraphics *graphics=(EGLGraphics*)context;
	// EGL_SetWindowTitle( graphics->wnd,bbApp().title.c_str() );
}

EGLGraphics::EGLGraphics( int w,int h ){
	for( int k=0;k<256;++k ) gamma_red[k]=gamma_green[k]=gamma_blue[k]=k;

	window_width=drawable_width=w;
	window_height=drawable_height=h;
	((GLCanvas*)front_canvas)->resize( w,h,1.0 );
	((GLCanvas*)back_canvas)->resize( w,h,1.0 );

	bbAppOnChange.add( onAppChange,this );
}

EGLGraphics::~EGLGraphics(){
	bbAppOnChange.remove( onAppChange,this );

	// EGL_GL_DeleteContext( context );
	// EGL_DestroyWindow( wnd );wnd=0;
}

void EGLGraphics::backup(){
}

bool EGLGraphics::restore(){
	return true;
}

void EGLGraphics::vwait(){}

void EGLGraphics::copy( BBCanvas *dest,int dx,int dy,int dw,int dh,BBCanvas *src,int sx,int sy,int sw,int sh ){
	dest->blit( dx,dy,src,sx,sy,sw,sh,false );
}

void EGLGraphics::setGamma( int r,int g,int b,float dr,float dg,float db ){
	gamma_red[r&255]=dr*257.0f;
	gamma_red[g&255]=dg*257.0f;
	gamma_red[b&255]=db*257.0f;
}

void EGLGraphics::getGamma( int r,int g,int b,float *dr,float *dg,float *db ){
	*dr=gamma_red[r&255];*dg=gamma_green[g&255];*db=gamma_blue[b&255];
}

void EGLGraphics::updateGamma( bool calibrate ){
	// EGL_SetWindowGammaRamp( wnd,gamma_red,gamma_green,gamma_blue );
}

//ACCESSORS
int EGLGraphics::getWidth()const{ return drawable_width; }
int EGLGraphics::getHeight()const{ return drawable_height; }
int EGLGraphics::getLogicalWidth()const{ return window_width; };
int EGLGraphics::getLogicalHeight()const{ return window_height; };
int EGLGraphics::getDepth()const{ return 0; }
float EGLGraphics::getDensity()const{ return (float)drawable_width/window_width; };
int EGLGraphics::getScanLine()const{ return 0; }
int EGLGraphics::getAvailVidmem()const{ return 0; }
int EGLGraphics::getTotalVidmem()const{ return 0; }

void EGLGraphics::moveMouse( int x,int y ){
	// EGL_WarpMouseInWindow( wnd,x,y );
}

/////////

EGLContextDriver::EGLContextDriver(){
	bbSceneDriver=this;

#ifdef BB_NX
	egl.create( nwindowGetDefault(),0 );
#endif
}

EGLContextDriver::~EGLContextDriver(){
	egl.destroy();
}

int EGLContextDriver::numGraphicsDrivers(){
	// return EGL_GetNumVideoDisplays();
	return 1;
}

void EGLContextDriver::graphicsDriverInfo( int driver,std::string *name,int *c ){
	// TODO
}

int EGLContextDriver::numGraphicsModes( int driver ){
	// return EGL_GetNumDisplayModes( driver );
	return 1;
}

void EGLContextDriver::graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c ){
	// EGL_DisplayMode dm;
	// EGL_GetDisplayMode( driver,mode,&dm );

	// *w=dm.w;
	// *h=dm.h;
	// *d=32; // TODO: https://wiki.libsdl.org/EGL_PixelFormatEnum
	// *c=GFXMODECAPS_3D;
}

void EGLContextDriver::windowedModeInfo( int *c ){
	*c=GFXMODECAPS_3D;
}

BBGraphics *EGLContextDriver::openGraphics( int w,int h,int d,int driver,int flags ){
	if( graphics ) return 0;

// 	bool inited=false;
// 	if( !inited ){
// 		if( EGL_Init(EGL_INIT_VIDEO)<0 ){
// 			LOGD( "%s","failed to init sdl" );
// 			return 0;
// 		}

// #ifdef BB_DESKTOP
// 		EGL_GL_SetAttribute( EGL_GL_CONTEXT_PROFILE_MASK,EGL_GL_CONTEXT_PROFILE_CORE );
// 		EGL_GL_SetAttribute( EGL_GL_CONTEXT_MAJOR_VERSION,3 );
// 		EGL_GL_SetAttribute( EGL_GL_CONTEXT_MINOR_VERSION,3 );
// #else
// 		EGL_GL_SetAttribute( EGL_GL_CONTEXT_MAJOR_VERSION,3 );
// 		EGL_GL_SetAttribute( EGL_GL_CONTEXT_MINOR_VERSION,0 );
// #endif
// 		inited=true;
// 	}

// 	EGL_Window* wnd=EGL_CreateWindow( bbApp().title.c_str(),EGL_WINDOWPOS_UNDEFINED,EGL_WINDOWPOS_UNDEFINED,1,1,EGL_WINDOW_OPENGL|EGL_WINDOW_ALLOW_HIGHDPI );
// 	if( wnd==NULL ){
// 		LOGD( "%s","failed to create window" );
// 		return 0;
// 	}

// 	bool windowed=flags & BBGraphics::GRAPHICS_WINDOWED ? true : false;
// 	bool scaled=windowed && (flags & BBGraphics::GRAPHICS_SCALED ? true : false);

// #ifndef BB_DESKTOP
// 	// no point in 'windowed' on mobile, right?
// 	windowed=false;
// 	scaled=false;
// #else
// 	EGL_DisplayMode mode;
// 	EGL_GetCurrentDisplayMode( 0,&mode );
// 	EGL_SetWindowPosition( wnd,(mode.w-w)/2.0f,(mode.h-h)/2.0f );
// 	EGL_SetWindowSize( wnd,w,h );
// 	EGL_SetWindowResizable( wnd,scaled?EGL_TRUE:EGL_FALSE );
// #endif
// 	EGL_ShowWindow( wnd );

// 	EGL_GLContext context;
// 	if( !(context=EGL_GL_CreateContext( wnd )) ){
// 		LOGD( "%s",EGL_GetError() );
// 		return 0;
// 	}

// 	EGL_GL_MakeCurrent( wnd,context );

// 	int screen_w,screen_h;
// 	int drawableW,drawableH;

// 	EGL_GetWindowSize( wnd,&screen_w,&screen_h );
// 	EGL_GL_GetDrawableSize( wnd,&drawableW,&drawableH );

// 	LOGD( "GL Version:  %s",glGetString( GL_VERSION ) );
// 	LOGD( "GL Vendor:   %s",glGetString( GL_VENDOR ) );
// 	LOGD( "GL window:   %ix%i",screen_w,screen_h );
// 	LOGD( "GL drawable: %ix%i",drawableW,drawableH );

// 	GL( glViewport( 0.0,0.0,drawableW,drawableH ) );
// 	GL( glScissor( 0.0,0.0,drawableW,drawableH ) );
// 	GL( glClearColor( 0.0,0.0,0.0,1.0 ) );
// 	GL( glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT ) );

// #ifdef BB_DESKTOP
// 	if( glewInit()!=GLEW_OK ){
// 		LOGD( "%s","failed to init glew" );
// 		return 0;
// 	}
// #endif

// 	if( windowed ){
// 		EGL_SetWindowFullscreen( wnd,0 );
// 	} else {
// 		EGL_SetWindowFullscreen( wnd,EGL_WINDOW_FULLSCREEN );
// 	}

	graphics=d_new EGLGraphics( w,h );
	// if( graphics->init() ){
		return graphics;
	// }
	// delete graphics;
	// return 0;
}

void EGLContextDriver::closeGraphics( BBGraphics *g ){
	if( graphics!=g || !g ) return;
	delete graphics;graphics=0;
}

bool EGLContextDriver::graphicsLost(){
	return false;
}

void EGLContextDriver::flip( bool vwait ){
	egl.swap();
}

static BBContextDriver *createEGLContext( const std::string &name ){
	return d_new EGLContextDriver();
}

BBMODULE_CREATE( context_egl ){
	bbContextDrivers.push_back( createEGLContext );

	return true;
}

BBMODULE_DESTROY( context_egl ){
	return true;
}
