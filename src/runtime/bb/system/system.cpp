
#include "../../stdutil/stdutil.h"
#include <bb/blitz/blitz.h>
#include <bb/runtime/runtime.h>
#include "system.h"
using namespace std;

map<string,string> bbSystemProperties;
BBSystemDriver *sys_driver;

void BBCALL bbRuntimeError( BBStr *str ){
	string t=*str;delete str;
	if( t.size()>255 ) t[255]=0;
	static char err[256];
	strcpy( err,t.c_str() );
	RTEX( err );
}

int BBCALL bbExecFile( BBStr *f ){
	string t=*f;delete f;
	int n=sys_driver->execute( t );
	if( !bbRuntimeIdle() ) RTEX( 0 );
	return n;
}

void BBCALL bbDelay( int ms ){
	if( !sys_driver->delay( ms ) ) RTEX( 0 );
}

int BBCALL bbMilliSecs(){
	return sys_driver->getMilliSecs();
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

int BBCALL bbScreenWidth( int i ){
	return sys_driver->getScreenWidth( i );
}

int BBCALL bbScreenHeight( int i ){
	return sys_driver->getScreenHeight( i );
}


BBMODULE_CREATE( system ){
	sys_driver=0;
	return true;
}

BBMODULE_DESTROY( system ){
	if( sys_driver ){
		delete sys_driver;
		sys_driver=0;
	}
	return true;
}
