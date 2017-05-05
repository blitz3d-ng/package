
#include "../../stdutil/stdutil.h"
#include <bb/blitz/module.h>
#include <FreeImage.h>
#include <cstring>
#include "pixmap.h"

using namespace std;

#ifndef WIN32
  #define WRONG_DIV '\\'
  #define RIGHT_DIV '/'
#else
  #define RIGHT_DIV '\\'
  #define WRONG_DIV '/'
#endif

BBPixmap::BBPixmap():width(0),height(0),depth(0),pitch(0),bits(0){
}

BBPixmap::~BBPixmap(){
  delete bits;
}

BBPixmap *bbLoadPixmap( const std::string &file ){
  string f;
  for( int i=0;i<file.size();i++ ){
    f+=file[i] == WRONG_DIV ? RIGHT_DIV : file[i];
  }

  FreeImage_Initialise();
  FREE_IMAGE_FORMAT fmt=FreeImage_GetFileType( f.c_str(),f.size() );
  if( fmt==FIF_UNKNOWN ) return 0;
  // if( fmt==FIF_UNKNOWN ){
  // 	int n=f.find( "." );if( n==string::npos ) return 0;
  // 	fmt=FreeImage_GetFileTypeFromExt( f.substr(n+1).c_str() );
  // 	if( fmt==FIF_UNKNOWN ) return 0;
  // }

  FIBITMAP *t_dib=FreeImage_Load( fmt,f.c_str(),0 );
  if( !t_dib ) return 0;

  bool trans=FreeImage_GetBPP( t_dib )==32 ||	FreeImage_IsTransparent( t_dib );

  FIBITMAP *dib=FreeImage_ConvertTo32Bits( t_dib );

  if( dib ) FreeImage_Unload( t_dib );
  else dib=t_dib;

  BBPixmap *pm=d_new BBPixmap();

  pm->width=FreeImage_GetWidth( dib );
  pm->height=FreeImage_GetHeight( dib );
  pm->pitch=FreeImage_GetPitch( dib );
  pm->bpp=FreeImage_GetBPP( dib )/8;

  int size=pm->width*pm->height*pm->bpp;
  pm->bits=new unsigned char[size];
  memcpy( pm->bits,FreeImage_GetBits( dib ),size );

  // NASTY: gotta be a better way to do this...
  for( int i=0;i<pm->width*pm->height;i++ ){
    unsigned char *p=&pm->bits[pm->bpp*i],tmp;
    tmp=p[0]; // ARGB? BGRA
    p[0]=p[2];
    p[2]=tmp;
  }

  FreeImage_Unload( dib );

  return pm;
}

void BBPixmap::mask( int r,int g,int b ){
	for( int i=0;i<width*height;i++ ){
		unsigned char *p=&bits[bpp*i];
		if( p[0]==0 && p[1]==0 && p[2]==0 ) p[3]=0.0f;
	}
}

BBMODULE_CREATE( pixmap ){
  return true;
}

BBMODULE_DESTROY( pixmap ){
  return true;
}
