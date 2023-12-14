
#include "../stdutil/stdutil.h"
#include <bb/blitz/blitz.h>
#include <bb/runtime/runtime.h>
#include "system.h"

#include <unistd.h>
#include <string.h>
#include <time.h>
#ifndef BB_WINDOWS
#include <sys/time.h>
#endif

#ifdef BB_MACOS
#include <CoreServices/CoreServices.h>
#include <CoreGraphics/CoreGraphics.h>
#endif
#ifdef BB_IOS
extern "C" {
	int iosScreenWidth();
	int iosScreenHeight();
}
#endif

static int base_time=0;
std::map<std::string,std::string> bbSystemProperties;
BBSystemDriver *bbSystemDriver=0;

void BBCALL bbRuntimeError( BBStr *str ){
	std::string t=*str;delete str;
	if( t.size()>255 ) t[255]=0;
	static char err[256];
	strcpy( err,t.c_str() );
	RTEX( err );
}

bb_int_t BBCALL bbExecFile( BBStr *f ){
	std::string t=*f;delete f;
	if( !t.size() ) return false;

	int n=0;
#ifdef BB_WINDOWS
	//convert cmd_line to cmd and params
	std::string cmd=t,params;
	while( cmd.size() && cmd[0]==' ' ) cmd=cmd.substr( 1 );
	if( cmd.find( '\"' )==0 ){
		int n=cmd.find( '\"',1 );
		if( n!=std::string::npos ){
			params=cmd.substr( n+1 );
			cmd=cmd.substr( 1,n-1 );
		}
	}else{
		int n=cmd.find( ' ' );
		if( n!=std::string::npos ){
			params=cmd.substr( n+1 );
			cmd=cmd.substr( 0,n );
		}
	}
	while( params.size() && params[0]==' ' ) params=params.substr( 1 );
	while( params.size() && params[params.size()-1]==' ' ) params=params.substr( 0,params.size()-1 );

	SetForegroundWindow( GetDesktopWindow() );

	n=(bb_int_t)ShellExecute( GetDesktopWindow(),0,cmd.c_str(),params.size() ? params.c_str() : 0,0,SW_SHOW )>32;
#else
	// TODO: ...
#endif
	if( !bbRuntimeIdle() ) RTEX( 0 );
	return n;
}

void BBCALL bbDelay( bb_int_t ms ){
	int t=bbMilliSecs()+ms;
	for(;;){
		if( !bbRuntimeIdle() ) RTEX( 0 );
		int d=t-bbMilliSecs();	//how long left to wait
		if( d<=0 ) return;
		if( d>100 ) d=100;
#ifdef BB_WINDOWS
		Sleep( d );
#else
		usleep( d * 1000 );
#endif
	}
}

bb_int_t BBCALL bbMilliSecs(){
#ifdef BB_WINDOWS
	return timeGetTime();
#else
	int t;
	struct timeval tv;
	gettimeofday(&tv,0);
	t=tv.tv_sec*1000;
	t+=tv.tv_usec/1000;
	return t-base_time;
#endif
}

BBStr * BBCALL bbSystemProperty( BBStr *p ){
	std::string t=tolower(*p);
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
#ifdef BB_WINDOWS
	return GetSystemMetrics( SM_CXSCREEN );
#endif
#ifdef BB_MACOS
	auto id=CGMainDisplayID();
	return CGDisplayPixelsWide( id );
#endif
#ifdef BB_IOS
	return iosScreenWidth();
#endif
	// TODO: linux,android,ovr,emscripten,nx
	return 0;
}

bb_int_t BBCALL bbScreenHeight( bb_int_t i ){
#ifdef BB_WINDOWS
	return GetSystemMetrics( SM_CYSCREEN );
#endif
#ifdef BB_MACOS
	auto id=CGMainDisplayID();
	return CGDisplayPixelsHigh( id );
#endif
#ifdef BB_IOS
	return iosScreenHeight();
#endif
	// TODO: linux,android,ovr,emscripten,nx
	return 0;
}

#ifdef BB_WINDOWS
static bool dpi_calculated=false;
static float dpi_x, dpi_y;
void calcDPI(){
	if( dpi_calculated ) return;

	HDC hdc=GetDC( GetDesktopWindow() );
	dpi_x=GetDeviceCaps( hdc,LOGPIXELSX ) / 96.0f;
	dpi_y=GetDeviceCaps( hdc,LOGPIXELSY ) / 96.0f;
	ReleaseDC( GetDesktopWindow(),hdc );
	dpi_calculated=true;
}
#endif

bb_float_t BBCALL bbDPIScaleX(){
#ifdef BB_WINDOWS
	calcDPI();return dpi_x;
#endif
#ifdef BB_MACOS
	return 2.0; // TODO:
#endif
	return 1.0;
}

bb_float_t BBCALL bbDPIScaleY(){
#ifdef BB_WINDOWS
	calcDPI();return dpi_y;
#endif
#ifdef BB_MACOS
	return 2.0; // TODO:
#endif
	return 1.0;
}

BBStr * BBCALL bbCurrentDate(){
	time_t t;
	time( &t );
	char buff[256];
	strftime( buff,256,"%d %b %Y",localtime( &t ) );
	return d_new BBStr( buff );
}

BBStr * BBCALL bbCurrentTime(){
	time_t t;
	time( &t );
	char buff[256];
	strftime( buff,256,"%H:%M:%S",localtime( &t ) );
	return d_new BBStr( buff );
}

BBMODULE_CREATE( system ){
	bbSystemDriver=0;
	bbSystemProperties["appdir"]=bbApp().executable_path;
#ifndef BB_WINDOWS
	base_time=bbMilliSecs();
#endif
	return true;
}

BBMODULE_DESTROY( system ){
	if( bbSystemDriver ){
		delete bbSystemDriver;
		bbSystemDriver=0;
	}
	return true;
}
