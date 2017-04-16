
#include "../../stdutil/stdutil.h"
#include "system.macos.h"
#include <unistd.h>

bool MacOSSystemDriver::delay( int ms ){
  usleep( ms * 1000 );
  return true;
}

bool MacOSSystemDriver::execute( const std::string &cmd ){
  return false;
}

int MacOSSystemDriver::getMilliSecs(){
  return 0;
}

int MacOSSystemDriver::getScreenWidth( int i ){
  return 0;
}

int MacOSSystemDriver::getScreenHeight( int i ){
  return 0;
}

void MacOSSystemDriver::dpiInfo( float &scale_x,float &scale_y ){
  scale_x=scale_y=1.0f;
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
