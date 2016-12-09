
#include "driver.h"
#include <windows.h>

int WindowsSystemDriver::getScreenWidth( int i ){
  return GetSystemMetrics( SM_CXSCREEN );
}

int WindowsSystemDriver::getScreenHeight( int i ){
  return GetSystemMetrics( SM_CYSCREEN );
}
