
#ifndef GXGRAPHICS_H
#define GXGRAPHICS_H

#include "graphics.d3d7.h"
#include <bb/blitz3d.d3d7/blitz3d.d3d7.h>

#include <set>
#include <string>
#include <d3d.h>

class gxGraphics : public D3D7B3DGraphics, public BBGraphics{
public:
	IDirectDraw7 *dirDraw;
	IDirectDraw *ds_dirDraw;

	gxGraphics( IDirectDraw7 *dirDraw,IDirectDrawSurface7 *front,IDirectDrawSurface7 *back,bool d3d );
	~gxGraphics();

	bool init();

	void backup();
	bool restore();

	static void wipeSystemProperties();
	void setSystemProperties();
private:

	BBFont *def_font;
	bool gfx_lost;

	DDSURFACEDESC2 initDesc( int w,int h,int flags );
	ddSurf *createSurface( int width,int height,int flags );
	ddSurf *loadSurface( const std::string &f,int flags );

	std::set<std::string> font_res;

	DDGAMMARAMP _gammaRamp;
	IDirectDrawGammaControl *_gamma;

	/***** GX INTERFACE *****/
public:
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

	BBMovie *openMovie( const std::string &file,int flags );

	BBFont *loadFont( const std::string &font,int height,int flags );

	// b3dgraphics
	IDirectDraw7 *getDirDraw(){ return dirDraw; }
	D3D7Canvas *backCanvas(){ return (D3D7Canvas*)back_canvas; }
};

#endif
