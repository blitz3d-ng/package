
#include "../stdutil/stdutil.h"
#include "system.ndk.h"
#include <unistd.h>

bool NDKSystemDriver::delay( int ms ){
	usleep( ms*1000 );
	return true;
}

bool NDKSystemDriver::execute( const std::string &cmd ){
	return false;
}

int NDKSystemDriver::getMilliSecs(){
	struct timespec ts;
	clock_gettime( CLOCK_REALTIME,&ts );

	return ts.tv_sec*1000+(ts.tv_nsec/1.0e6);
}

int NDKSystemDriver::getScreenWidth( int i ){
	return 0;
}

int NDKSystemDriver::getScreenHeight( int i ){
	return 0;
}

void NDKSystemDriver::dpiInfo( float &scale_x,float &scale_y ){
  scale_x=scale_y=1.0f;
}

bool NDKSystemDriver::lookupFontData( const std::string &fontName,BBFontData &font ){
	return false;
}

void NDKSystemDriver::setJNI( jobject klass,JNIEnv *e ){
	activity_class=klass;
	env=e;
}

jobject NDKSystemDriver::getActivityClass(){
	return activity_class;
}

JNIEnv *NDKSystemDriver::getEnv(){
	return env;
}

BBMODULE_CREATE( system_ndk ){
	if( !bbSystemDriver ){
		bbSystemDriver=d_new NDKSystemDriver();
	}
	return true;
}

BBMODULE_DESTROY( system_ndk ){
	return true;
}
