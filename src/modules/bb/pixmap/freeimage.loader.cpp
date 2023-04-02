
#include <bb/blitz/blitz.h>
#include <bb/filesystem/filesystem.h>
#include "../stdutil/stdutil.h"
#include "pixmap.h"
using namespace std;

#include <FreeImage.h>
#include <string.h>

unsigned DLL_CALLCONV readProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	return ((streambuf*)handle)->sgetn( (char*)buffer,size*count ) / size;
}

unsigned DLL_CALLCONV writeProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	return ((streambuf*)handle)->sputn( (char*)buffer,size*count );
}

int DLL_CALLCONV seekProc(fi_handle handle, long offset, int origin) {
	return ((streambuf*)handle)->pubseekoff( offset,(origin==SEEK_SET?ios_base::beg:(origin==SEEK_CUR?ios_base::cur:ios_base::end)) )==-1;
}

long DLL_CALLCONV tellProc(fi_handle handle) {
	return ((streambuf*)handle)->pubseekoff( 0,ios_base::cur );
}

BBPixmap *bbLoadPixmapWithFreeImage( const std::string &path ){
	static bool inited=false;
	if( !inited ){
		FreeImage_Initialise();
		inited=true;
	}

	FreeImageIO io;
	io.read_proc  = readProc;
	io.write_proc = writeProc;
	io.seek_proc  = seekProc;
	io.tell_proc  = tellProc;

	string f=path;
	std::streambuf *buf=gx_filesys->openFile( f,std::ios_base::in );
	if( !buf ){
		return 0;
	}

	FREE_IMAGE_FORMAT fmt = FreeImage_GetFileTypeFromHandle( &io,(fi_handle)buf,0 );
	if( fmt==FIF_UNKNOWN ){
		int n=f.find( "." );if( n==string::npos ){ delete buf;return 0; }
		fmt=FreeImage_GetFIFFromFilename( f.substr(n+1).c_str() );
		if( fmt==FIF_UNKNOWN ){ delete buf;return 0; }
	}

	FIBITMAP *t_dib=FreeImage_LoadFromHandle( fmt,&io,(fi_handle)buf,0 );
	if( !t_dib ){ delete buf;return 0; }

	bool trans=FreeImage_GetBPP( t_dib )==32 ||	FreeImage_IsTransparent( t_dib );

	BBPixmap *pm=d_new BBPixmap();

	switch( int bpp=FreeImage_GetBPP( t_dib ) ){
	case 32:pm->format=PF_RGBA;break;
	case 24:pm->format=PF_RGB;break;
	case 8:case 4:pm->format=PF_RGB;break;
	default:RTEX( ("Unhandled image format: "+string(itoa(bpp))+" bpps").c_str() );
	}

	FIBITMAP *dib=FreeImage_ConvertTo32Bits( t_dib );

	if( dib ) FreeImage_Unload( t_dib );
	else dib=t_dib;

	pm->width=FreeImage_GetWidth( dib );
	pm->height=FreeImage_GetHeight( dib );
	pm->pitch=FreeImage_GetPitch( dib );
	pm->bpp=FreeImage_GetBPP( dib )/8;

	int scanline_size=pm->width*pm->bpp;
	int size=scanline_size*pm->height;
	pm->bits=new unsigned char[size];
	for( int y=0;y<pm->height;y++ ){
		memcpy( pm->bits+((pm->height-y-1)*scanline_size),FreeImage_GetScanLine( dib,y ),scanline_size );
	}

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
