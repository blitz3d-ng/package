
#ifndef GXCANVAS_H
#define GXCANVAS_H

#include <bb/graphics/canvas.h>
#include "font.h"
#include "ddutil.h"

typedef IDirectDrawSurface7 ddSurf;

class D3D7Canvas : public BBCanvas{
public:
	D3D7Canvas( IDirectDraw7 *dirDraw,IDirectDrawSurface7 *surface,int flags );
	virtual ~D3D7Canvas();

	void backup()const;
	void restore()const;
	ddSurf *getSurface()const;
	ddSurf *getTexSurface()const;
	void setModify( int n );
	int getModify()const;

	bool attachZBuffer( DDPIXELFORMAT zbuffFmt );
	void releaseZBuffer();
	bool getZBufferFormat( DDPIXELFORMAT &fmt );

	virtual void damage( const RECT &r )const;

	void unset(){}
	void set(){}
protected:
	int cube_mode;

	IDirectDraw7 *dirDraw;
	ddSurf *main_surf,*surf,*z_surf,*cube_surfs[6];

	mutable int mod_cnt;
	mutable ddSurf *t_surf;

	mutable int locked_pitch,locked_cnt,lock_mod_cnt,remip_cnt;
	mutable unsigned char *locked_surf;

	PixelFormat format;
};

class gxCanvas : public D3D7Canvas{
public:
	gxCanvas( IDirectDraw7 *dirDraw,ddSurf *surface,BBFont *font,int flags );
	~gxCanvas();

	void damage( const RECT &r )const;

private:
	bool clip( RECT *d )const;
	bool clip( RECT *d,RECT *s )const;

	mutable int cm_pitch;
	mutable unsigned *cm_mask;

	RECT clip_rect;

	D3D7Font *font;
	RECT viewport;
	int origin_x,origin_y,handle_x,handle_y;
	unsigned mask_surf,color_surf,color_argb,clsColor_surf;

	void updateBitMask( const RECT &r )const;

	/***** GX INTERFACE *****/
public:
	//MANIPULATORS
	void setFont( BBFont *font );
	void setMask( unsigned argb );
	void setColor( unsigned argb );
	void setClsColor( unsigned argb );
	void setOrigin( int x,int y );
	void setHandle( int x,int y );
	void setViewport( int x,int y,int w,int h );

	void cls();
	void plot( int x,int y );
	void line( int x,int y,int x2,int y2 );
	void rect( int x,int y,int w,int h,bool solid );
	void oval( int x,int y,int w,int h,bool solid );
	void text( int x,int y,const std::string &t );
	void blit( int x,int y,BBCanvas *src,int src_x,int src_y,int src_w,int src_h,bool solid );
	void image( BBCanvas *c,int x,int y,bool solid );

	bool collide( int x,int y,const BBCanvas *src,int src_x,int src_y,bool solid )const;
	bool rect_collide( int x,int y,int rect_x,int rect_y,int rect_w,int rect_h,bool solid )const;

	bool lock()const;
	void setPixel( int x,int y,unsigned argb );
	void setPixelFast( int x,int y,unsigned argb ){
		format.setPixel( locked_surf+y*locked_pitch+x*format.getPitch(),argb );
		++mod_cnt;
	}
	void copyPixel( int x,int y,BBCanvas *src,int src_x,int src_y );
	void copyPixelFast( int x,int y,BBCanvas *src,int src_x,int src_y );
	unsigned getPixel( int x,int y )const;
	unsigned getPixelFast( int x,int y )const;
	void unlock()const;

	void setCubeMode( int mode );
	void setCubeFace( int face );

	//ACCESSORS
	int getWidth()const;
	int getHeight()const;
	int getDepth()const;
	int cubeMode()const{ return cube_mode; }
	void getOrigin( int *x,int *y )const;
	void getHandle( int *x,int *y )const;
	void getViewport( int *x,int *y,int *w,int *h )const;
	unsigned getMask()const;
	unsigned getColor()const;
	unsigned getClsColor()const;
};

#endif
