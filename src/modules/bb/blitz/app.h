#ifndef BLITZ_APP_H
#define BLITZ_APP_H

#include "basic.h"
#include <bb/hook/hook.h>
#include <string>

struct BBApp{
  std::string title,close;
  std::string executable_path, cmd_line;
};

extern BBHook bbAppOnChange;

void BBCALL bbStartup( const char *executable_path,const char *params );
BBApp BBCALL bbApp();

#ifdef BB_IOS
#include <syslog.h>
#define _bbLog( ... ) syslog( LOG_WARNING, __VA_ARGS__ )
#else
#define _bbLog( ... ) fprintf( stderr, __VA_ARGS__ )
#endif

#endif
