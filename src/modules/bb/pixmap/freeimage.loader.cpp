
#include <bb/blitz/blitz.h>
#include "../../stdutil/stdutil.h"
#include "pixmap.h"
using namespace std;

#include <FreeImage.h>
#include <string.h>

BBPixmap *bbLoadPixmapWithFreeImage( const std::string &file ){
	string f=file;
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

  BBPixmap *pm=d_new BBPixmap();

	switch( int bpp=FreeImage_GetBPP( t_dib ) ){
	case 32:pm->format=PF_RGBA;break;
	case 24:pm->format=PF_RGB;break;
	case 8:pm->format=PF_RGB;break;
	default:RTEX( ("Unhandled image format: "+string(itoa(bpp))+" bpps").c_str() );
	}

	FIBITMAP *dib=FreeImage_ConvertTo32Bits( t_dib );

	if( dib ) FreeImage_Unload( t_dib );
	else dib=t_dib;

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
