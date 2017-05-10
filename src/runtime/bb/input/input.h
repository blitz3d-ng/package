
#ifndef BBINPUT_H
#define BBINPUT_H

#include <bb/blitz/blitz.h>
#include <bb/input/driver.h>
#include <vector>

extern std::vector<BBDevice*> bbJoysticks;
extern BBInputDriver *gx_input;

int BBCALL bbEnumInput();

#include "commands.h"

#endif
