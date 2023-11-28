
#include "../stdutil/stdutil.h"
#include "system.nx.h"

#include <sys/time.h>
//#include <sys/sysinfo.h>

#include <unistd.h>
#include <switch.h>

#include <fstream>

static int base_time=0;

bool NXSystemDriver::delay( int ms ){
  usleep( ms * 1000 );
  return true;
}

bool NXSystemDriver::execute( const std::string &cmd ){
  return false;
}

int NXSystemDriver::getMilliSecs(){
	int t;
	struct timeval tv;
	gettimeofday(&tv,0);
	t=tv.tv_sec*1000;
	t+=tv.tv_usec/1000;
	return t-base_time;
}

int NXSystemDriver::getScreenWidth( int i ){
  return 0;
}

int NXSystemDriver::getScreenHeight( int i ){
  return 0;
}

void NXSystemDriver::dpiInfo( float &scale_x,float &scale_y ){
	scale_x=scale_y=1.0f;
}

bool NXSystemDriver::lookupFontData( const std::string &fontName,BBFontData &font ){
	return false;
}

BBMODULE_CREATE( system_nx ){
	if( !bbSystemDriver ){
		bbSystemDriver=d_new NXSystemDriver();
		base_time=bbMilliSecs();
	}
	return true;
}

BBMODULE_DESTROY( system_nx ){
	return true;
}
