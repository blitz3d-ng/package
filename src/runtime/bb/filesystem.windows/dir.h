
#ifndef GXDIR_H
#define GXDIR_H

#include <string>
#include <windows.h>

#include <bb/filesystem/driver.h>

class WindowsDir : public BBDir{
public:
	WindowsDir( HANDLE h,const WIN32_FIND_DATA &f );
	~WindowsDir();

private:
	HANDLE handle;
	WIN32_FIND_DATA findData;

	/***** GX INTERFACE *****/
public:
	std::string getNextFile();
};

#endif
