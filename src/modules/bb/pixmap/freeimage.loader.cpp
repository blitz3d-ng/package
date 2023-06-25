
#include <bb/blitz/blitz.h>
#include <bb/filesystem/filesystem.h>
#include "../stdutil/stdutil.h"
#include "pixmap.h"

#include <FreeImage.h>
#include <string.h>

unsigned DLL_CALLCONV readProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	return ((std::streambuf*)handle)->sgetn( (char*)buffer,size*count ) / size;
}

unsigned DLL_CALLCONV writeProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	return ((std::streambuf*)handle)->sputn( (char*)buffer,size*count );
}

int DLL_CALLCONV seekProc(fi_handle handle, long offset, int origin) {
	return ((std::streambuf*)handle)->pubseekoff( offset,(origin==SEEK_SET?std::ios_base::beg:(origin==SEEK_CUR?std::ios_base::cur:std::ios_base::end)) )==-1;
}

long DLL_CALLCONV tellProc(fi_handle handle) {
	return ((std::streambuf*)handle)->pubseekoff( 0,std::ios_base::cur );
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

	std::string f=path;
	std::streambuf *buf=gx_filesys->openFile( f,std::ios_base::in );
	if( !buf ){
		return 0;
	}

	FREE_IMAGE_FORMAT fmt = FreeImage_GetFileTypeFromHandle( &io,(fi_handle)buf,0 );
	if( fmt==FIF_UNKNOWN ){
		int n=f.find( "." );if( n==std::string::npos ){ delete buf;return 0; }
		fmt=FreeImage_GetFIFFromFilename( f.substr(n+1).c_str() );
		if( fmt==FIF_UNKNOWN ){ delete buf;return 0; }
	}

	FIBITMAP *t_dib=FreeImage_LoadFromHandle( fmt,&io,(fi_handle)buf,0 );
	if( !t_dib ){ delete buf;return 0; }


	BBPixmap *pm=d_new BBPixmap();
	pm->trans=FreeImage_GetBPP( t_dib )==32||FreeImage_IsTransparent( t_dib );

	// TODO: it would be nice, from a memory perspective, to
	// support other pixel formats beyond RGBA.
	// switch( int bpp=FreeImage_GetBPP( t_dib ) ){
	// case 32:pm->format=PF_RGBA;break;
	// case 24:pm->format=PF_RGB;break;
	// case 8:case 4:pm->format=PF_RGB;break;
	// default:RTEX( ("Unhandled image format: "+std::string(itoa(bpp))+" bpps").c_str() );
	// }

	FIBITMAP *dib=FreeImage_ConvertTo32Bits( t_dib );
	pm->format=PF_RGBA;

	if( dib ) FreeImage_Unload( t_dib );
	else dib=t_dib;

	pm->width=FreeImage_GetWidth( dib );
	pm->height=FreeImage_GetHeight( dib );
	pm->pitch=FreeImage_GetPitch( dib );
	pm->bpp=FreeImage_GetBPP( dib )/8;

	int size=pm->width*pm->bpp*pm->height;
	pm->bits=new unsigned char[size];
	memcpy( pm->bits,FreeImage_GetBits( dib ),size );

	FreeImage_Unload( dib );
	delete buf;

	return pm;
}
