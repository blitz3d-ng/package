#ifndef BB_RUNTIME_SDL_H
#define BB_RUNTIME_SDL_H

#include <bb/runtime/runtime.h>
#include <bb/graphics.sdl/graphics.sdl.h>
#include <bb/blitz3d.gl/blitz3d.gl.h>

#include <SDL.h>

class SDLGraphics;

class SDLRuntime : public GLB3DGraphics, public BBRuntime, public SDLContextDriver{
private:
	SDL_Window* wnd;
	SDLGraphics *graphics;

	static void _refreshTitle( void *data,void *context );
	void setTitle( const char *title );

	void resize();
public:
	SDLRuntime( SDL_Window *wnd );
	~SDLRuntime();

	void afterCreate();

	// context driver
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
