#ifndef BB_BLITZ2D_GL_H
#define BB_BLITZ2D_GL_H

#include <bb/graphics.gl/graphics.gl.h>
#include <bb/pixmap/pixmap.h>

class GLCanvas : public BBCanvas{
protected:
	void backup()const{}

	ContextResources *res;

	int width,height;
	mutable unsigned char *pixels;
	BBImageFont *font;

	float scale_x,scale_y;
	int origin_x,origin_y;
	int handle_x,handle_y;
	float color[3];

	virtual void bind()const=0;

	void quad( int x,int y,int w,int h,bool solid,bool tex,float tx,float ty,float color[3] );
public:
	GLCanvas( ContextResources *res,int f );

	void resize( int w,int h,float d );

	virtual unsigned int framebufferId()=0;

	//MANIPULATORS
	void setFont( BBFont *f );
	void setMask( unsigned argb );
	void setColor( unsigned argb );
	void setClsColor( unsigned argb );
	void setOrigin( int x,int y );
	void setScale( float x,float y );
	void setHandle( int x,int y );
	void setViewport( int x,int y,int w,int h );

	void cls();
	void plot( int x,int y );
	void line( int x,int y,int x2,int y2 );
	void rect( int x,int y,int w,int h,bool solid );
	void oval( int x,int y,int w,int h,bool solid );
	void text( int x,int y,const std::string &t );
	void blit( int x,int y,BBCanvas *s,int src_x,int src_y,int src_w,int src_h,bool solid );
	void image( BBCanvas *c,int x,int y,bool solid );

	bool collide( int x,int y,const BBCanvas *src,int src_x,int src_y,bool solid )const;
	bool rect_collide( int x,int y,int rect_x,int rect_y,int rect_w,int rect_h,bool solid )const;

	bool lock()const;
	void setPixel( int x,int y,unsigned argb );
	void setPixelFast( int x,int y,unsigned argb );
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
	int cubeMode()const;
	void getOrigin( int *x,int *y )const;
	void getScale( float *x,float *y )const;
	void getHandle( int *x,int *y )const;
	void getViewport( int *x,int *y,int *w,int *h )const;
	unsigned getMask()const;
	unsigned getColor()const;
	unsigned getClsColor()const;
};

class GLTextureCanvas : public GLCanvas{
protected:
	unsigned int texture,framebuffer,depthbuffer;
	int twidth,theight;
public:
	GLTextureCanvas( ContextResources *res,int f );
	GLTextureCanvas( ContextResources *res,int w,int h,int f );
	GLTextureCanvas( ContextResources *res,BBPixmap *pixmap,int f );

	int getDepth()const;

	void set();

	void unset();

	void uploadData( void *data );

	unsigned int textureId();
	unsigned int framebufferId();

	void setPixmap( BBPixmap *pm );

	void bind()const;
};

class GLDefaultCanvas : public GLCanvas{
protected:
	int mode;
	unsigned int framebuffer;

	void bind()const;
public:
	GLDefaultCanvas( ContextResources *res,unsigned int fb,int m,int f );

	void unset();
	void set();

	unsigned int framebufferId();

	void blit( int x,int y,BBCanvas *src,int src_x,int src_y,int src_w,int src_h,bool solid );

	int getDepth()const;
};

#endif
