#ifndef BBWINDOWS_SYSTEM_DRIVER
#define BBWINDOWS_SYSTEM_DRIVER

#include <string>
#include "../system/driver.h"

class WindowsSystemDriver : public BBSystemDriver{
public:
	~WindowsSystemDriver();

	int getScreenWidth( int i );
	int getScreenHeight( int i );

	int callDll( const std::string &dll,const std::string &func,const void *in,int in_sz,void *out,int out_sz );
};

#endif
