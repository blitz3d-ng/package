#ifndef BB_RUNTIME_SDL_H
#define BB_RUNTIME_SDL_H

#include <bb/runtime/runtime.h>
#include <bb/graphics.sdl/graphics.sdl.h>
#include <bb/blitz3d.gl/blitz3d.gl.h>

#include <SDL.h>

class SDLGraphics;

class SDLRuntime :public BBRuntime{
private:
	SDLGraphics *graphics;

	static void _refreshTitle( void *data,void *context );
	void setTitle( const char *title );
public:
	SDLRuntime();
	~SDLRuntime();

	void afterCreate();

	void asyncStop();
	void asyncRun();
	void asyncEnd();

	bool idle();

	void *window();

	void moveMouse( int x,int y );
	void setPointerVisible( bool vis );
};

#endif
