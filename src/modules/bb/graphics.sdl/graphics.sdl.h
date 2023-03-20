#ifndef BB_GRAPHICS_SDL_H
#define BB_GRAPHICS_SDL_H

#include <bb/graphics.gl/graphics.gl.h>
#include <SDL.h>

class SDLContextDriver:public BBContextDriver{
public:
	int numGraphicsDrivers();
	void graphicsDriverInfo( int driver,std::string *name,int *c );
	int numGraphicsModes( int driver );
	void graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c );
	void windowedModeInfo( int *c );
};

#endif
