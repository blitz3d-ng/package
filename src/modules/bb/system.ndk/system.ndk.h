#ifndef BB_SYSTEM_ANDROID_H
#define BB_SYSTEM_ANDROID_H

#include <bb/system/system.h>
#include <jni.h>

class NDKSystemDriver : public BBSystemDriver{
private:
	jobject activity_class;
	JNIEnv *env;
public:
	bool delay( int ms );

	bool execute( const std::string &cmd );
	int getMilliSecs();

	int getScreenWidth( int i );
	int getScreenHeight( int i );

	void dpiInfo( float &scale_x,float &scale_y );

	bool lookupFontData( const std::string &fontName,BBFontData &font );

	void setJNI( jobject klass,JNIEnv *e );

	jobject getActivityClass();
	JNIEnv *getEnv();
};

#endif
