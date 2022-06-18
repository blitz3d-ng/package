
#include "../../stdutil/stdutil.h"
#include <bb/blitz/blitz.h>
#include <bb/runtime/runtime.h>
#include "system.h"
#include <cstring>
using namespace std;

map<string,string> bbSystemProperties;
BBSystemDriver *bbSystemDriver=0;

void BBCALL bbRuntimeError( BBStr *str ){
	string t=*str;delete str;
	if( t.size()>255 ) t[255]=0;
	static char err[256];
	strcpy( err,t.c_str() );
	RTEX( err );
}

bb_int_t BBCALL bbExecFile( BBStr *f ){
	string t=*f;delete f;
	int n=bbSystemDriver->execute( t );
	if( !bbRuntimeIdle() ) RTEX( 0 );
	return n;
}

void BBCALL bbDelay( bb_int_t ms ){
	if( !bbSystemDriver->delay( ms ) ) RTEX( 0 );
}

bb_int_t BBCALL bbMilliSecs(){
	return bbSystemDriver->getMilliSecs();
}

BBStr * BBCALL bbSystemProperty( BBStr *p ){
	string t=tolower(*p);
	delete p;return d_new BBStr( bbSystemProperties[t] );
}

BBStr * BBCALL bbGetEnv( BBStr *env_var ){
	char *p=getenv( env_var->c_str() );
	BBStr *val=d_new BBStr( p ? p : "" );
	delete env_var;
	return val;
}

void BBCALL bbSetEnv( BBStr *env_var,BBStr *val ){
#ifndef _WIN32
	setenv( (*env_var).c_str(),(*val).c_str(),1 );
#else
	std::string t=*env_var+"="+*val;
	_putenv( t.c_str() );
#endif
	delete env_var;
	delete val;
}

bb_int_t BBCALL bbScreenWidth( bb_int_t i ){
	return bbSystemDriver->getScreenWidth( i );
}

bb_int_t BBCALL bbScreenHeight( bb_int_t i ){
	return bbSystemDriver->getScreenHeight( i );
}

bb_float_t BBCALL bbDPIScaleX(){
	float x,y;
	bbSystemDriver->dpiInfo( x,y );
	return x;
}

bb_float_t BBCALL bbDPIScaleY(){
	float x,y;
	bbSystemDriver->dpiInfo( x,y );
	return y;
}

BBMODULE_CREATE( system ){
	bbSystemDriver=0;
	bbSystemProperties["appdir"]=bbApp().executable_path;
	return true;
}

BBMODULE_DESTROY( system ){
	if( bbSystemDriver ){
		delete bbSystemDriver;
		bbSystemDriver=0;
	}
	return true;
}
