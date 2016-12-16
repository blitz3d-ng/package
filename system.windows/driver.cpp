
#include <map>
#include <windows.h>

using namespace std;

#include "../stdutil/stdutil.h"
#include "driver.h"


typedef int (_stdcall *LibFunc)( const void *in,int in_sz,void *out,int out_sz );

struct gxDll{
	HINSTANCE hinst;
	map<string,LibFunc> funcs;
};

static map<string,gxDll*> libs;

WindowsSystemDriver::~WindowsSystemDriver(){
  map<string,gxDll*>::const_iterator it;
  for( it=libs.begin();it!=libs.end();++it ){
    FreeLibrary( it->second->hinst );
  }
  libs.clear();
}

int WindowsSystemDriver::getScreenWidth( int i ){
  return GetSystemMetrics( SM_CXSCREEN );
}

int WindowsSystemDriver::getScreenHeight( int i ){
  return GetSystemMetrics( SM_CYSCREEN );
}

int WindowsSystemDriver::callDll( const std::string &dll,const std::string &func,const void *in,int in_sz,void *out,int out_sz ){

	map<string,gxDll*>::const_iterator lib_it=libs.find( dll );

	if( lib_it==libs.end() ){
		HINSTANCE h=LoadLibrary( dll.c_str() );
		if( !h ) return 0;
		gxDll *t=d_new gxDll;
		t->hinst=h;
		lib_it=libs.insert( make_pair( dll,t ) ).first;
	}

	gxDll *t=lib_it->second;
	map<string,LibFunc>::const_iterator fun_it=t->funcs.find( func );

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
}
