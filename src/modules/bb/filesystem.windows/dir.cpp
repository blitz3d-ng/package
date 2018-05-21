
#include <windows.h>
#include "dir.h"
using namespace std;

WindowsDir::WindowsDir( HANDLE h,const WIN32_FIND_DATA &f ):handle(h),findData(f){
}

WindowsDir::~WindowsDir(){
	if( handle!=INVALID_HANDLE_VALUE ) FindClose( handle );
}

string WindowsDir::getNextFile(){
	if( handle==INVALID_HANDLE_VALUE ) return "";
	string t=findData.cFileName;
	if( !FindNextFile( handle,&findData ) ){
		FindClose( handle );
		handle=INVALID_HANDLE_VALUE;
	}
	return t;
}
