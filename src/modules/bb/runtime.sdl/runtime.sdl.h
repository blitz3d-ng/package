#ifndef BB_RUNTIME_SDL_H
#define BB_RUNTIME_SDL_H

#include <bb/runtime/runtime.h>
#include <bb/graphics/graphics.h>
#ifdef BB_DESKTOP
#include <bb/blitz2d.gl/blitz2d.gl.h>
#include <bb/blitz3d.gl/blitz3d.gl.h>
#else
#include <bb/blitz2d.gles2/blitz2d.gles2.h>
#include <bb/blitz3d.gles2/blitz3d.gles2.h>
// TODO: not the best approach...
#define GLB3DGraphics      GLES2B3DGraphics
#define GLB2DTextureCanvas GLES2B2DTextureCanvas
#define GLB2DDefaultCanvas GLES2B2DDefaultCanvas
#define GLB2DCanvas        GLES2B2DCanvas
#endif
#include <SDL.h>

class SDLGraphics;

class SDLRuntime : public GLB3DGraphics, public BBRuntime, public BBContextDriver{
private:
	SDL_Window* wnd;
	SDLGraphics *graphics;

	static void _refreshTitle( void *data,void *context );
	void setTitle( const char *title );

	void resize( int width,int height );
public:
	SDLRuntime( SDL_Window *wnd );
	~SDLRuntime();

	// context driver
	int numGraphicsDrivers();
	void graphicsDriverInfo( int driver,std::string *name,int *c );
	int numGraphicsModes( int driver );
	void graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c );
	void windowedModeInfo( int *c );

	BBGraphics *openGraphics( int w,int h,int d,int driver,int flags );
	void closeGraphics( BBGraphics *graphics );
	bool graphicsLost();

	void flip( bool vwait );

	// runtime
	void asyncStop();
	void asyncRun();
	void asyncEnd();

	bool idle();

	void *window();

	void moveMouse( int x,int y );
	void setPointerVisible( bool vis );
};

#endif
