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
#define LOGD( fmt,... ) syslog( LOG_WARNING,fmt,__VA_ARGS__ )
#else
#define LOGD( fmt,... ) fprintf( stderr,fmt"\n",__VA_ARGS__ )
#endif

#endif
