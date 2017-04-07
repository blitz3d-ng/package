#ifndef BBSYSTEM_DRIVER_H
#define BBSYSTEM_DRIVER_H

#include <bb/blitz/module.h>

class BBSystemDriver{
public:
  virtual int getScreenWidth( int i )=0;
  virtual int getScreenHeight( int i )=0;
};

int BBCALL bbScreenWidth( int i );
int BBCALL bbScreenHeight( int i );

extern BBSystemDriver *sys_driver;

#endif
