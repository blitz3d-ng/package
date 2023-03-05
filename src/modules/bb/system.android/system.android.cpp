
#include "../stdutil/stdutil.h"
#include "system.android.h"
#include <unistd.h>

bool AndroidSystemDriver::delay( int ms ){
	usleep( ms*1000 );
	return true;
}

bool AndroidSystemDriver::execute( const std::string &cmd ){
	return false;
}

int AndroidSystemDriver::getMilliSecs(){
	struct timespec ts;
	clock_gettime( CLOCK_REALTIME,&ts );

	return ts.tv_sec*1000+(ts.tv_nsec/1.0e6);
}

int AndroidSystemDriver::getScreenWidth( int i ){
	return 0;
}

int AndroidSystemDriver::getScreenHeight( int i ){
	return 0;
}

void AndroidSystemDriver::dpiInfo( float &scale_x,float &scale_y ){
  scale_x=scale_y=1.0f;
}

bool AndroidSystemDriver::lookupFontData( const std::string &fontName,BBFontData &font ){
	return false;
}

BBMODULE_CREATE( system_android ){
	if( !bbSystemDriver ){
		bbSystemDriver=d_new AndroidSystemDriver();
	}
	return true;
}

BBMODULE_DESTROY( system_android ){
	return true;
}
