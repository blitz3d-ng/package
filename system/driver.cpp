
#include "../stdutil/stdutil.h"
#include "driver.h"

BBSystemDriver *sys_driver;

// TODO: figure a more elegant way to handle this...
#if defined(WIN32) || defined(WIN64)
#include "../system.windows/driver.h"
#endif

int BBCALL bbScreenWidth( int i ){
  return sys_driver->getScreenWidth( i );
}

int BBCALL bbScreenHeight( int i ){
  return sys_driver->getScreenHeight( i );
}

BBMODULE_CREATE( system ){
#if defined(WIN32) || defined(WIN64)
  sys_driver=d_new WindowsSystemDriver();
  return true;
#else
  return false;
#endif
}

BBMODULE_DESTROY( system ){
  return true;
}

BBMODULE_LINK( system ){
  rtSym( "%ScreenWidth%i=-1",bbScreenWidth );
  rtSym( "%ScreenHeight%i=-1",bbScreenHeight );
}
