#ifndef BBWINDOWS_SYSTEM_DRIVER
#define BBSYSTEM_WINDOWS_DRIVER

#include "../system/driver.h"

class WindowsSystemDriver : public BBSystemDriver{
public:
  int getScreenWidth( int i );
  int getScreenHeight( int i );
};

#endif
