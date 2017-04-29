#ifndef BB_BLITZ2D_GL_H
#define BB_BLITZ2D_GL_H

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

  int width,height,flags;
	mutable unsigned char *pixels;

  virtual void bind()const=0;

public:
  GLB2DCanvas( int f ):width(0),height(0),pixels(0),flags(f){
  }

  void resize( int w,int h ){
    width=w;height=h;
  }

  //MANIPULATORS
  void setFont( BBFont *font ){}
  void setMask( unsigned argb ){}
  void setColor( unsigned argb ){
    int r = (argb >> 16) & 255;
    int g = (argb >> 8) & 255;
    int b = argb & 255;

    glColor3f( r/255.0f,g/255.0f,b/255.0f );
  }
  void setClsColor( unsigned argb ){
    int r = (argb >> 16) & 255;
    int g = (argb >> 8) & 255;
    int b = argb & 255;

    glClearColor( r/255.0f,g/255.0f,b/255.0f,1.0f );
  }
  void setOrigin( int x,int y ){}
  void setHandle( int x,int y ){}
  void setViewport( int x,int y,int w,int h ){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0.0f,w,h,0.0f,-1.0f,1.0f );

    glViewport( 0,0,w,h );
    std::cout<<"viewport: "<<w<<", "<<h<<std::endl;
  }

  void cls(){
    glClear(GL_COLOR_BUFFER_BIT);
  }
  void plot( int x,int y ){
    glBegin( GL_POINTS );
      glVertex2f( x,y );
    glEnd();
  }
  void line( int x,int y,int x2,int y2 ){
    glBegin( GL_LINES );
      glVertex2f( x,y );
      glVertex2f( x2,y2 );
    glEnd();
  }
  void rect( int x,int y,int w,int h,bool solid ){
    glPointSize( 5.0f );
    glBegin( GL_QUADS );
      glTexCoord2f( 0.0f,0.0f );
      glVertex2f( x,y+h );
      glTexCoord2f( 1.0f,0.0f );
      glVertex2f( x+w,y+h );
      glTexCoord2f( 1.0f,1.0f );
      glVertex2f( x+w,y );
      glTexCoord2f( 0.0f,1.0f );
      glVertex2f( x,y );
    glEnd();
  }
  void oval( int x,int y,int w,int h,bool solid ){}
  void text( int x,int y,const std::string &t ){}
  void blit( int x,int y,BBCanvas *src,int src_x,int src_y,int src_w,int src_h,bool solid ){

    ((GLB2DCanvas*)src)->bind();

    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT );

    rect( x,y,src_w,src_h,solid );

    glDisable( GL_TEXTURE_2D );
  }

  bool collide( int x,int y,const BBCanvas *src,int src_x,int src_y,bool solid )const{ return false; }
  bool rect_collide( int x,int y,int rect_x,int rect_y,int rect_w,int rect_h,bool solid )const{ return false; }

  bool lock()const{
		if( pixels ) return false;

		pixels=new unsigned char[width*height*4];

		bind();
		glGetTexImage( GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels );
		return true;
	}
  void setPixel( int x,int y,unsigned argb ){}
  void setPixelFast( int x,int y,unsigned argb ){}
  void copyPixel( int x,int y,BBCanvas *src,int src_x,int src_y ){}
  void copyPixelFast( int x,int y,BBCanvas *src,int src_x,int src_y ){}
  unsigned getPixel( int x,int y )const{ return 0; }
  unsigned getPixelFast( int x,int y )const{
    return pixels[(y*width+x)*4];
  }
  void unlock()const{
		free(pixels);
	}

  void setCubeMode( int mode ){}
  void setCubeFace( int face ){}

  //ACCESSORS
  int getWidth()const{ return width; }
  int getHeight()const{ return height; }
  int getDepth()const{ return 0; }
  int getFlags()const{ return flags; }
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
	GLB2DTextureCanvas( int f ):GLB2DCanvas( f ){
		glGenTextures( 1,&texture );
		glGenFramebuffers( 1,&framebuffer );
		glGenRenderbuffers( 1,&depthbuffer );
	}

	GLB2DTextureCanvas( int w,int h,int f ):GLB2DTextureCanvas(f){
		width=w;
		height=h;
	}

  GLB2DTextureCanvas( BBPixmap *pixmap,int f ):GLB2DCanvas(f),texture(0),framebuffer(0),depthbuffer(0){
		flags=f;

    if( pixmap ) setPixmap( pixmap );
  }

  unsigned int getTextureId(){ return texture; }

	void setPixmap( BBPixmap *pm ){
		width=pm->width;
		height=pm->height;

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
  void bind()const{}
public:
  GLB2DDefaultCanvas( int f ):GLB2DCanvas(f){}
};

#endif
