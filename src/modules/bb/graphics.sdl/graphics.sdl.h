#ifndef BB_GRAPHICS_SDL_H
#define BB_GRAPHICS_SDL_H

#include <bb/graphics.gl/graphics.gl.h>
#include <bb/blitz3d.gl/blitz3d.gl.h>
#include <SDL.h>

class SDLContextDriver:public GLB3DGraphics,public BBContextDriver{
public:
	SDLContextDriver();

	int numGraphicsDrivers();
	void graphicsDriverInfo( int driver,std::string *name,int *c );
	int numGraphicsModes( int driver );
	void graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c );
	void windowedModeInfo( int *c );

	BBGraphics *openGraphics( int w,int h,int d,int driver,int flags );
	void closeGraphics( BBGraphics *graphics );
	bool graphicsLost();

	void flip( bool vwait );
};

class SDLGraphics : public BBGraphics{
protected:
	int window_width,window_height,drawable_width,drawable_height;

	unsigned short gamma_red[256], gamma_green[256], gamma_blue[256];

	BBImageFont *def_font;
public:
	SDL_Window *wnd;

	SDLGraphics( SDL_Window *wnd );
	~SDLGraphics();

	void resize();

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

	BBFont *getDefaultFont()const;

	//OBJECTS
	BBCanvas *createCanvas( int width,int height,int flags );
	BBCanvas *loadCanvas( const std::string &file,int flags );

	// b2dgraphics
	BBMovie *openMovie( const std::string &file,int flags );
	void moveMouse( int x,int y );
};


#endif
