#ifndef BLITZ_APP_H
#define BLITZ_APP_H

#include "basic.h"
#include <bb/hook/hook.h>

struct BBApp{
  std::string title,close;
  std::string cmd_line;
};

extern BBHook bbAppOnChange;

void BBCALL bbStartup( std::string cmd_line );
BBApp BBCALL bbApp();

#endif
