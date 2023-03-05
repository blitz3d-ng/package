
#ifndef GXGRAPHICS_H
#define GXGRAPHICS_H

#include <set>
#include <string>
#include <d3d.h>

#include <bb/blitz3d/graphics.h>
#include <bb/blitz2d/graphics.h>
#include <bb/graphics.d3d7/canvas.h>
#include <bb/blitz3d.d3d7/scene.h>
#include <bb/blitz3d.d3d7/mesh.h>
#include "gxmovie.h"

class gxGraphics : public B3DGraphics, public B2DGraphics{
public:
	IDirectDraw7 *dirDraw;
	IDirectDraw *ds_dirDraw;

	IDirect3D7 *dir3d;
	IDirect3DDevice7 *dir3dDev;
	D3DDEVICEDESC7 dir3dDevDesc;
	DDPIXELFORMAT primFmt,zbuffFmt;

	DDPIXELFORMAT texRGBFmt[2],texAlphaFmt[2],texRGBAlphaFmt[2],texRGBMaskFmt[2];

	gxGraphics( IDirectDraw7 *dirDraw,IDirectDrawSurface7 *front,IDirectDrawSurface7 *back,bool d3d );
	~gxGraphics();

	void backup();
	bool restore();

	static void wipeSystemProperties();
	void setSystemProperties();
private:

	BBFont *def_font;
	bool gfx_lost;
	gxMesh *dummy_mesh;

	DDSURFACEDESC2 initDesc( int w,int h,int flags );
	ddSurf *createSurface( int width,int height,int flags );
	ddSurf *loadSurface( const std::string &f,int flags );

	std::set<BBFont*> font_set;
	std::set<gxScene*> scene_set;
	std::set<gxMovie*> movie_set;
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

	BBCanvas *getFrontCanvas()const;
	BBCanvas *getBackCanvas()const;
	BBFont *getDefaultFont()const;

	//OBJECTS
	BBCanvas *createCanvas( int width,int height,int flags );
	BBCanvas *loadCanvas( const std::string &file,int flags );

	BBMovie *openMovie( const std::string &file,int flags );
	BBMovie *verifyMovie( BBMovie *movie );
	void closeMovie( BBMovie *movie );

	BBFont *loadFont( const std::string &font,int height,int flags );
	BBFont *verifyFont( BBFont *font );
	void freeFont( BBFont *font );

	BBScene *createScene( int w,int h,float d,int flags );
	BBScene *verifyScene( BBScene *scene );
	void freeScene( BBScene *scene );
};

#endif
