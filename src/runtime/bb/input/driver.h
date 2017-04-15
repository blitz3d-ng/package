#ifndef BB_INPUT_H
#define BB_INPUT_H

#include "device.h"

class BBInputDriver{
  /***** GX INTERFACE *****/
public:
  enum{
		ASC_HOME=1,ASC_END=2,ASC_INSERT=3,ASC_DELETE=4,
		ASC_PAGEUP=5,ASC_PAGEDOWN=6,
		ASC_UP=28,ASC_DOWN=29,ASC_RIGHT=30,ASC_LEFT=31
	};

  virtual BBDevice *getJoystick( int port )const=0;
  virtual int getJoystickType( int port )const=0;
  virtual int numJoysticks()const=0;
  virtual int toAscii( int key )const=0;
};


#endif
