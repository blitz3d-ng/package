#include "graphics.sdl.h"

int SDLContextDriver::numGraphicsDrivers(){
	return SDL_GetNumVideoDisplays();
}

void SDLContextDriver::graphicsDriverInfo( int driver,std::string *name,int *c ){
}

int SDLContextDriver::numGraphicsModes( int driver ){
	return SDL_GetNumDisplayModes( driver );
}

void SDLContextDriver::graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c ){
	SDL_DisplayMode dm;
	SDL_GetDisplayMode( driver,mode,&dm );

	*w=dm.w;
	*h=dm.h;
	*d=32; // TODO: https://wiki.libsdl.org/SDL_PixelFormatEnum
	*c=GFXMODECAPS_3D;
}

void SDLContextDriver::windowedModeInfo( int *c ){
	*c=GFXMODECAPS_3D;
}

BBMODULE_EMPTY( graphics_sdl )
