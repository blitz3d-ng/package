#ifndef BB_BLITZ2D_GL_H
#define BB_BLITZ2D_GL_H

#include <iostream>
using namespace std;

#include <bb/blitz2d/blitz2d.h>
#include <bb/pixmap/pixmap.h>

#if defined(__linux__) || defined(_WIN32)
#include <GL/glew.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
// #include <GL/glext.h>
#endif

#include <iostream>

class GLB2DCanvas : public BBCanvas{
protected:
  void backup()const{}

  int width,height;
	mutable unsigned char *pixels;
  BBImageFont *font;

	int handle_x,handle_y;
	float color[3];

  virtual void bind()const=0;

public:
	GLB2DCanvas( int f ):width(0),height(0),pixels(0),handle_x(0),handle_y(0){
		flags=f;
	}

	void resize( int w,int h ){
		width=w;height=h;
	}

	virtual unsigned int framebufferId()=0;

	//MANIPULATORS
	void setFont( BBFont *f );
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
	void blit( int x,int y,BBCanvas *s,int src_x,int src_y,int src_w,int src_h,bool solid );
	void image( BBCanvas *c,int x,int y,bool solid );

  bool collide( int x,int y,const BBCanvas *src,int src_x,int src_y,bool solid )const{ return false; }
  bool rect_collide( int x,int y,int rect_x,int rect_y,int rect_w,int rect_h,bool solid )const{ return false; }

  bool lock()const{
		if( pixels ) return false;

		pixels=new unsigned char[width*height*4];

		bind();
		glGetTexImage( GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels );
		return true;
	}
	void setPixel( int x,int y,unsigned argb );
	void setPixelFast( int x,int y,unsigned argb );
  void copyPixel( int x,int y,BBCanvas *src,int src_x,int src_y ){}
  void copyPixelFast( int x,int y,BBCanvas *src,int src_x,int src_y ){}
  unsigned getPixel( int x,int y )const{ return 0; }
  unsigned getPixelFast( int x,int y )const{
    y=height-y;
    return pixels[(y*width+x)*4];
  }
  void unlock()const{
		delete[] pixels;pixels=0;
	}

  void setCubeMode( int mode ){}
  void setCubeFace( int face ){}

  //ACCESSORS
  int getWidth()const{ return width; }
  int getHeight()const{ return height; }
  int cubeMode()const{ return 0; }
  void getOrigin( int *x,int *y )const{}
  void getHandle( int *x,int *y )const{}
  void getViewport( int *x,int *y,int *w,int *h )const{
    *x=0;*y=0;*w=getWidth();*h=getHeight();
  }
  unsigned getMask()const{ return 0; }
  unsigned getColor()const{ return 0; }
  unsigned getClsColor()const{ return 0; }

};

class GLB2DTextureCanvas : public GLB2DCanvas{
protected:
  unsigned int texture,framebuffer,depthbuffer;
public:
	GLB2DTextureCanvas( int f ):GLB2DCanvas( f ),texture(0),framebuffer(0),depthbuffer(0){
	}

	GLB2DTextureCanvas( int w,int h,int f ):GLB2DTextureCanvas(f){
		width=w;
		height=h;
	}

  GLB2DTextureCanvas( BBPixmap *pixmap,int f ):GLB2DTextureCanvas(f){
    if( pixmap ) setPixmap( pixmap );
  }

	int getDepth()const{ return 8; }

	void unset(){
		glFlush();

		glBindTexture( GL_TEXTURE_2D,texture );
		glGenerateMipmap( GL_TEXTURE_2D );
	}

	unsigned int textureId(){
		if( texture ) return texture;

		glGenTextures( 1,&texture );
		glBindTexture( GL_TEXTURE_2D,texture );
		glTexParameteri( GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE );
		glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,0 );

		return texture;
	}

	unsigned int framebufferId(){
		if( framebuffer ) return framebuffer;

		glGenRenderbuffers( 1,&depthbuffer );
		glBindRenderbuffer( GL_RENDERBUFFER,depthbuffer );
		glRenderbufferStorage( GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height );
		glBindRenderbuffer( GL_RENDERBUFFER,0 );

		glGenFramebuffers( 1,&framebuffer );
		glBindFramebuffer( GL_FRAMEBUFFER,framebuffer );
		glFramebufferTexture2D( GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,textureId(),0 );
		glFramebufferRenderbuffer( GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthbuffer );

		glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

		return framebuffer;
	}

	void set(){
		glBindFramebuffer( GL_FRAMEBUFFER,framebufferId() );
	}

	void setPixmap( BBPixmap *pm ){
		if( flags&CANVAS_TEX_ALPHA ){
			if( flags&CANVAS_TEX_MASK ){
				pm->mask( 0,0,0 );
			}else{
				pm->buildAlpha( false );
			}
		}

		width=pm->width;
		height=pm->height;

		if( !texture ) glGenTextures( 1,&texture );
		glBindTexture( GL_TEXTURE_2D,texture );
		glTexParameteri( GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE );
		glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,pm->width,pm->height,0,GL_RGBA,GL_UNSIGNED_BYTE,pm->bits );
	}

  void bind()const{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D,texture );
  }
};

class GLB2DDefaultCanvas : public GLB2DCanvas{
protected:
	int mode;

  void bind()const{}
public:
  GLB2DDefaultCanvas( int m,int f ):GLB2DCanvas(f),mode(m){
		set();
		glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	}

	void unset(){
		glFlush();
	}
	void set(){
		glBindFramebuffer( GL_FRAMEBUFFER,0 );
		glDrawBuffer( mode );
	}

	unsigned int framebufferId(){ return 0; }

	void blit( int x,int y,BBCanvas *src,int src_x,int src_y,int src_w,int src_h,bool solid ){
	}

	int getDepth()const{ return 8; }
};

#endif
