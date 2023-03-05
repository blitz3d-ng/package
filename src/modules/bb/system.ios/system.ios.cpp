
#include "../stdutil/stdutil.h"
#include "system.ios.h"

#include <CoreFoundation/CFDate.h>
#include <unistd.h>

#include <fstream>
using namespace std;

extern "C" {
	int iosScreenWidth();
	int iosScreenHeight();
}

bool iOSSystemDriver::delay( int ms ){
  usleep( ms * 1000 );
  return true;
}

bool iOSSystemDriver::execute( const std::string &cmd ){
  return false;
}

int iOSSystemDriver::getMilliSecs(){
	struct timespec ts;
	clock_gettime( CLOCK_UPTIME_RAW,&ts );

	return ts.tv_sec*1000+(ts.tv_nsec/1.0e6);
}

int iOSSystemDriver::getScreenWidth( int i ){
	return iosScreenWidth();
}

int iOSSystemDriver::getScreenHeight( int i ){
	return iosScreenHeight();
}

void iOSSystemDriver::dpiInfo( float &scale_x,float &scale_y ){
  scale_x=scale_y=1.0f;
}

extern "C" const char *lookupFontFile( const char *fontName );

bool iOSSystemDriver::lookupFontData( const std::string &fontName,BBFontData &font ){
	return false;
}

BBMODULE_CREATE( system_ios ){
	if( !bbSystemDriver ){
		bbSystemDriver=d_new iOSSystemDriver();
	}
	return true;
}

BBMODULE_DESTROY( system_ios ){
	return true;
}
