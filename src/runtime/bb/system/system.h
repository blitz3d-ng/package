#ifndef BB_SYSTEM_SYSTEM_H
#define BB_SYSTEM_SYSTEM_H

#include <bb/blitz/blitz.h>
#include <bb/blitz/module.h>
#include <map>

class BBSystemDriver{
public:
  virtual ~BBSystemDriver(){}

  virtual bool delay( int ms )=0;

  virtual bool execute( const std::string &cmd )=0;
  virtual int getMilliSecs()=0;

  virtual int getScreenWidth( int i )=0;
  virtual int getScreenHeight( int i )=0;
  virtual void dpiInfo( float &scale_x,float &scale_y )=0;
};

#include "commands.h"

extern BBSystemDriver *bbSystemDriver;
extern std::map<std::string,std::string> bbSystemProperties;

#endif
