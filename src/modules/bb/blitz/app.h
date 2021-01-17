#ifndef BLITZ_APP_H
#define BLITZ_APP_H

#include "basic.h"
#include <bb/hook/hook.h>

struct BBApp{
  std::string title,close;
  std::string executable_path, cmd_line;
};

extern BBHook bbAppOnChange;

void BBCALL bbStartup( const char *executable_path,const char *params );
BBApp BBCALL bbApp();

#endif
