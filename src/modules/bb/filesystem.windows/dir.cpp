
#include <windows.h>
#include "dir.h"

WindowsDir::WindowsDir( HANDLE h,const WIN32_FIND_DATA &f ):handle(h),findData(f){
}

WindowsDir::~WindowsDir(){
	if( handle!=INVALID_HANDLE_VALUE ) FindClose( handle );
}

std::string WindowsDir::getNextFile(){
	if( handle==INVALID_HANDLE_VALUE ) return "";
	std::string t=findData.cFileName;
	if( !FindNextFile( handle,&findData ) ){
		FindClose( handle );
		handle=INVALID_HANDLE_VALUE;
	}
	return t;
}
