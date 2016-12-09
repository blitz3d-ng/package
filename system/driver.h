#ifndef BBSYSTEM_DRIVER_H
#define BBSYSTEM_DRIVER_H

class BBSystemDriver{
public:
  virtual int getScreenWidth( int i )=0;
  virtual int getScreenHeight( int i )=0;
};

#endif
