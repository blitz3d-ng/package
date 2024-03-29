
#include <bb/blitz/blitz.h>
#include "pixmap.h"
#include "../../../stdutil/stdutil.h"

#include <string.h>

BBPixmap::BBPixmap():width(0),height(0),depth(0),pitch(0),bits(0){
}

BBPixmap::~BBPixmap(){
  delete bits;
}

int BBPixmap::getWidth(){
	return width;
}

int BBPixmap::getHeight(){
	return height;
}

int BBPixmap::read( int x,int y ){
	return bits[(y*width+x)*bpp];
}

// TODO: introduce a proper dynamic loader system like BlitzMax.
BBPixmap *bbLoadPixmapWithFreeImage( const std::string &file );
extern "C" BBPixmap *bbLoadPixmapWithUIKit( const char *file );

BBPixmap *bbLoadPixmap( const std::string &file ){
	std::string f=canonicalpath( file );

	BBPixmap *pix=0;
#ifdef BB_IOS
	pix=bbLoadPixmapWithUIKit( f.c_str() );
#else
	pix=bbLoadPixmapWithFreeImage( f );
#endif

	// if( pix ){
	// 	LOGD( "[pixmap] Loaded %s", f.c_str() );
	// }else{
	// 	LOGD( "[pixmap] Failed to load %s", f.c_str() );
	// }

	return pix;
}

void BBPixmap::mask( int r,int g,int b ){
	for( int i=0;i<width*height;i++ ){
		unsigned char *p=&bits[bpp*i];
		if( p[2]==r && p[1]==g && p[0]==b ) p[3]=0.0f;
	}
}

void BBPixmap::buildAlpha( bool whiten ){
	// NASTY: gotta be a better way to do this...
	for( int i=0;i<width*height;i++ ){
		unsigned char *p=&bits[bpp*i];
		p[3]=(p[0]+p[1]+p[2])/3;
	}
}

void BBPixmap::fill( int r,int g,int b,float a ){
	for( int i=0;i<width*height;i++ ){
		unsigned char *p=&bits[bpp*i];
		p[0]=b;
		p[1]=g;
		p[2]=r;
		p[3]=a*255;
	}
}

void BBPixmap::flipVertically(){
	int scanline_size=width*bpp;
	int size=scanline_size*height;

	auto tmp=new unsigned char[size];
	for( int y=0;y<height;y++ ){
		memcpy( tmp+((height-y-1)*scanline_size),bits+(y*scanline_size),scanline_size );
	}

	memcpy( bits,tmp,size );
	delete[] tmp;
}

void BBPixmap::swapBytes0and2(){
	// NASTY: gotta be a better way to do this...
	for( int i=0;i<width*height;i++ ){
		unsigned char *p=&bits[bpp*i],tmp;
		tmp=p[0]; // ARGB? BGRA
		p[0]=p[2];
		p[2]=tmp;
	}
}

BBMODULE_CREATE( pixmap ){
  return true;
}

BBMODULE_DESTROY( pixmap ){
  return true;
}
