
#include "../stdutil/stdutil.h"
#include <bb/blitz/module.h>
#include <bb/runtime/runtime.h>
#include "system.windows.h"

#include <windows.h>
#include <map>

typedef int (__stdcall *LibFunc)( const void *in,int in_sz,void *out,int out_sz );

struct gxDll{
	HINSTANCE hinst;
	std::map<std::string,LibFunc> funcs;
};

static std::map<std::string,gxDll*> libs;
static OSVERSIONINFO osinfo;

WindowsSystemDriver::WindowsSystemDriver(){
}

WindowsSystemDriver::~WindowsSystemDriver(){
	TIMECAPS tc;
	timeGetDevCaps( &tc,sizeof(tc) );
	timeEndPeriod( tc.wPeriodMin );

	std::map<std::string,gxDll*>::const_iterator it;
	for( it=libs.begin();it!=libs.end();++it ){
		FreeLibrary( it->second->hinst );
	}
	libs.clear();
}

bool WindowsSystemDriver::isXPorLess(){
	return osinfo.dwMajorVersion<6;
}

static std::string toDir( std::string t ){
	if( t.size() && t[t.size()-1]!='\\' ) t+='\\';
	return t;
}

void WindowsSystemDriver::refreshSystemProperties(){
	char buff[MAX_PATH+1];

	bbSystemProperties["cpu"]="Intel";

	std::string os="Unknown";
	switch( osinfo.dwMajorVersion ){
	case 3:
		switch( osinfo.dwMinorVersion ){
		case 51:os="Windows NT 3.1";break;
		}
		break;
	case 4:
		switch( osinfo.dwMinorVersion ){
		case 0:os="Windows 95";break;
		case 10:os="Windows 98";break;
		case 90:os="Windows ME";break;
		}
		break;
	case 5:
		switch( osinfo.dwMinorVersion ){
		case 0:os="Windows 2000";break;
		case 1:os="Windows XP";break;
		case 2:os="Windows Server 2003";break;
		}
		break;
	case 6:
		switch( osinfo.dwMinorVersion ){
		case 0:os="Windows Vista";break;
		case 1:os="Windows 7";break;
		case 2:os="Windows 8";break;
		case 3:os="Windows 8.1";break;
		}
	case 10:
		switch( osinfo.dwMinorVersion ){
		case 0:os="Windows 10";break;
		}
		break;
	}

	bbSystemProperties["os"]=os;

	if( GetModuleFileName( 0,buff,MAX_PATH ) ){
		std::string t=buff;
		int n=t.find_last_of( '\\' );
		if( n!=std::string::npos ) t=t.substr( 0,n );
		bbSystemProperties["appdir"]=toDir( t );
	}

	if( GetWindowsDirectory( buff,MAX_PATH ) ) bbSystemProperties["windowsdir"]=toDir( buff );
	if( GetSystemDirectory( buff,MAX_PATH ) )  bbSystemProperties["systemdir"]=toDir( buff );
	if( GetTempPath( MAX_PATH,buff ) )         bbSystemProperties["tempdir"]=toDir( buff );
}

// TODO: figure out if this is broken on x64.
int callDll( const std::string &dll,const std::string &func,const void *in,int in_sz,void *out,int out_sz ){

#ifndef BB_WIN32
	RTEX( "Only available in 32-bit builds." );
	return 0;
#else
	std::map<std::string,gxDll*>::const_iterator lib_it=libs.find( dll );

	if( lib_it==libs.end() ){
		HINSTANCE h=LoadLibrary( dll.c_str() );
		if( !h ) return 0;
		gxDll *t=d_new gxDll;
		t->hinst=h;
		lib_it=libs.insert( make_pair( dll,t ) ).first;
	}

	gxDll *t=lib_it->second;
	std::map<std::string,LibFunc>::const_iterator fun_it=t->funcs.find( func );

	if( fun_it==t->funcs.end() ){
		LibFunc f=(LibFunc)GetProcAddress( t->hinst,func.c_str() );
		if( !f ) return 0;
		fun_it=t->funcs.insert( make_pair( func,f ) ).first;
	}

	static void *save_esp;

	_asm{
		mov	[save_esp],esp
	};

	int n=fun_it->second( in,in_sz,out,out_sz );

	_asm{
		mov esp,[save_esp]
	};

	return n;
#endif
}

extern inline void debugBank( bbBank *b );
extern inline void debugBank( bbBank *b,int offset );

BBLIB bb_int_t BBCALL bbCallDLL( BBStr *dll,BBStr *fun,bbBank *in,bbBank *out ){
	if( bb_env.debug ){
		if( in ) debugBank( in );
		if( out ) debugBank( out );
	}
	int t=callDll( *dll,*fun,
		in ? in->data : 0,in ? in->size : 0,
		out ? out->data : 0,out ? out->size : 0 );
	delete dll;delete fun;
	return t;
}

BBMODULE_CREATE( system_windows ){
	memset( &osinfo,0,sizeof(osinfo) );
	osinfo.dwOSVersionInfoSize=sizeof(osinfo);
	GetVersionEx( &osinfo );

	TIMECAPS tc;
	timeGetDevCaps( &tc,sizeof(tc) );
	timeBeginPeriod( tc.wPeriodMin );

	if( !bbSystemDriver ){
		bbSystemDriver=d_new WindowsSystemDriver();
	}
	return true;
}

BBMODULE_DESTROY( system_windows ){
	return true;
}
