#ifndef BB_BLITZ2D_GL_H
#define BB_BLITZ2D_GL_H

#include <bb/graphics.gl/graphics.gl.h>
#include <bb/pixmap/pixmap.h>

class GLCanvas : public BBCanvas{
protected:
	void backup()const{}

	int width,height;
	float dpi;
	mutable unsigned char *pixels;
	BBImageFont *font;

	int origin_x,origin_y;
	int handle_x,handle_y;
	float color[3];

	virtual void bind()const=0;

	void quad( int x,int y,int w,int h,bool solid,bool tex,float tx,float ty,float color[3] );
public:
	GLCanvas( int f ):width(0),height(0),dpi(1.0),pixels(0),handle_x(0),handle_y(0){
		flags=f;
	}

	void resize( int w,int h,float d ){
		width=w;height=h;
		dpi=d;
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
#ifdef BB_DESKTOP
		GL( glGetTexImage( GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels ) );
#endif
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

class GLTextureCanvas : public GLCanvas{
protected:
	unsigned int texture,framebuffer,depthbuffer;
	int twidth,theight;
public:
	GLTextureCanvas( int f ):GLCanvas( f ),texture(0),framebuffer(0),depthbuffer(0),twidth(0),theight(0){
	}

	GLTextureCanvas( int w,int h,int f ):GLTextureCanvas(f){
		width=w;
		height=h;
	}

	GLTextureCanvas( BBPixmap *pixmap,int f ):GLTextureCanvas(f){
		if( pixmap ) setPixmap( pixmap );
	}

	int getDepth()const{ return 8; }

	void set(){
		GL( glBindFramebuffer( GL_FRAMEBUFFER,framebufferId() ) );
	}

	void unset(){
		GL( glFlush() );

		GL( glBindTexture( GL_TEXTURE_2D,texture ) );
		GL( glGenerateMipmap( GL_TEXTURE_2D ) );
	}

	void uploadData( void *data ){
		if( !texture ) GL( glGenTextures( 1,&texture ) );
		GL( glActiveTexture( GL_TEXTURE0 ) );
		GL( glBindTexture( GL_TEXTURE_2D,texture ) );
		GL( glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data ) );
		GL( glGenerateMipmap( GL_TEXTURE_2D ) );
	}

	unsigned int textureId(){
		if( texture ) return texture;

		uploadData(0);
		return texture;
	}

	unsigned int framebufferId(){
		if( framebuffer ) return framebuffer;

		GL( glGenRenderbuffers( 1,&depthbuffer ) );
		GL( glBindRenderbuffer( GL_RENDERBUFFER,depthbuffer ) );
		GL( glRenderbufferStorage( GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height ) );
		GL( glBindRenderbuffer( GL_RENDERBUFFER,0 ) );

		GL( glGenFramebuffers( 1,&framebuffer ) );
		GL( glBindFramebuffer( GL_FRAMEBUFFER,framebuffer ) );
		GL( glFramebufferTexture2D( GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,textureId(),0 ) );
		GL( glFramebufferRenderbuffer( GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthbuffer ) );

		GLenum status=GL( glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
		if( status!=GL_FRAMEBUFFER_COMPLETE ){
			_bbLog( "fb error: %s\n",bbGLFramebufferStatusString( status ) );
		}

		return framebuffer;
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
		uploadData( pm->bits );
		GL( glBindTexture( GL_TEXTURE_2D,0 ) );
	}

	void bind()const{
		GL( glBindTexture( GL_TEXTURE_2D,texture ) );
	}
};

class GLDefaultCanvas : public GLCanvas{
protected:
	int mode;
	unsigned int framebuffer;

	void bind()const;
public:
	GLDefaultCanvas( unsigned int fb,int m,int f );

	void unset();
	void set();

	unsigned int framebufferId();

	void blit( int x,int y,BBCanvas *src,int src_x,int src_y,int src_w,int src_h,bool solid );

	int getDepth()const;
};

#endif
