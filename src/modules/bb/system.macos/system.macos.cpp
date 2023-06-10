
#include "../stdutil/stdutil.h"
#include "system.macos.h"

#include <CoreServices/CoreServices.h>
#include <CoreGraphics/CoreGraphics.h>
#include <unistd.h>
#include <mach-o/dyld.h>

#include <fstream>

bool MacOSSystemDriver::delay( int ms ){
  usleep( ms * 1000 );
  return true;
}

bool MacOSSystemDriver::execute( const std::string &cmd ){
  return false;
}

// lifted from BlitzMax (https://github.com/blitz-research/blitzmax/blob/a97b18fd62b211ecee464097180e0ebf2e7f12b7/mod/brl.mod/blitz.mod/blitz_app.c#L83-L90)
int MacOSSystemDriver::getMilliSecs(){
	double t;
	UnsignedWide uw;
	Microseconds( &uw );
	t=(uw.hi<<(32-9))|(uw.lo>>9);	//divide by 512...!

	return (int) (long long) ( t/(1000.0/512.0) );
}

int MacOSSystemDriver::getScreenWidth( int i ){
	auto id=CGMainDisplayID();
	return CGDisplayPixelsWide( id );
}

int MacOSSystemDriver::getScreenHeight( int i ){
	auto id=CGMainDisplayID();
	return CGDisplayPixelsHigh( id );
}

void MacOSSystemDriver::dpiInfo( float &scale_x,float &scale_y ){
	// TODO: determine this automatically...
  scale_x=scale_y=2.0f;
}

extern "C" const char *lookupFontFile( const char *fontName );

bool MacOSSystemDriver::lookupFontData( const std::string &fontName,BBFontData &font ){
	const char *fontPath=lookupFontFile( fontName.c_str() );
	if( !fontPath ) return false;

	std::ifstream file( fontPath,std::ios::binary|std::ios::ate );
	font.size=file.tellg();
	file.seekg( 0,std::ios::beg );

	font.data=new unsigned char[font.size];
	if( file.read( (char*)font.data,font.size ) ){
		return true;
	}
	return false;
}

BBMODULE_CREATE( system_macos ){
	if( !bbSystemDriver ){
		bbSystemDriver=d_new MacOSSystemDriver();
	}
	return true;
}

BBMODULE_DESTROY( system_macos ){
	return true;
}
