#ifndef BB_SYSTEM_SYSTEM_H
#define BB_SYSTEM_SYSTEM_H

#include <bb/blitz/blitz.h>
#include <bb/blitz/module.h>
#include <map>

class BBSystemDriver{
public:
  virtual ~BBSystemDriver(){}
};

#include "commands.h"

extern BBSystemDriver *bbSystemDriver;
extern std::map<std::string,std::string> bbSystemProperties;

#endif
