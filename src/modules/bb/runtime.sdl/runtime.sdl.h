#ifndef BB_RUNTIME_SDL_H
#define BB_RUNTIME_SDL_H

#include <bb/runtime/runtime.h>
#include <bb/graphics/graphics.h>
#include <bb/blitz2d.gl/blitz2d.gl.h>
#include <bb/blitz3d.gl/blitz3d.gl.h>

#include <SDL.h>

class SDLGraphics;

class SDLRuntime : public GLB3DGraphics, public BBRuntime, public BBContextDriver{
private:
	SDL_Window* wnd;
	SDLGraphics *graphics;

	static void _refreshTitle( void *data,void *context );
	void setTitle( const char *title );

	void resize();
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
