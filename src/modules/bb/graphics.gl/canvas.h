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

	int mode;
	unsigned int framebuffer;

	float scale_x,scale_y;
	int origin_x,origin_y;
	int handle_x,handle_y;
	int mask;
	float color[3];
	BBPixmap *pixmap;
	bool dirty;

	void flush();

	void quad( int x,int y,int w,int h,bool solid,bool tex,float tx,float ty,float color[3] );
public:
	GLCanvas( ContextResources *res,int f );
	GLCanvas( ContextResources *res,int w,int h,int f );
	GLCanvas( ContextResources *res,BBPixmap *pixmap,int f );

	~GLCanvas();

	int cube_face,cube_mode;
	GLenum target;

	unsigned int texture,depthbuffer;
	int twidth,theight;

	void uploadData();
	void downloadData();
	void setPixmap( BBPixmap *pm );
	void setFramebuffer( unsigned int fb,int m );

	void resize( int w,int h,float d );

	void set();
	void unset();
	void bind();

	unsigned int textureId();
	unsigned int framebufferId();

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
	virtual void blit( int x,int y,BBCanvas *s,int src_x,int src_y,int src_w,int src_h,bool solid );
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
	int getDepth()const;
	int cubeMode()const;
	void getOrigin( int *x,int *y )const;
	void getScale( float *x,float *y )const;
	void getHandle( int *x,int *y )const;
	void getViewport( int *x,int *y,int *w,int *h )const;
	unsigned getMask()const;
	unsigned getColor()const;
	unsigned getClsColor()const;
};

#endif
