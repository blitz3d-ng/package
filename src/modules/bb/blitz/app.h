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
#define LOGI( fmt,... ) syslog( LOG_WARNING,fmt,__VA_ARGS__ )
#define LOGE( fmt,... ) syslog( LOG_WARNING,fmt,__VA_ARGS__ )
#define LOGD( fmt,... ) syslog( LOG_WARNING,fmt,__VA_ARGS__ )
#else

#ifdef BB_NDK
#include <android/log.h>
#define LOGI( fmt,... ) __android_log_print( ANDROID_LOG_INFO,"blitz3d",fmt,__VA_ARGS__ );
#define LOGE( fmt,... ) __android_log_print( ANDROID_LOG_ERROR,"blitz3d",fmt,__VA_ARGS__ );
#define LOGD( fmt,... ) __android_log_print( ANDROID_LOG_WARN,"blitz3d",fmt,__VA_ARGS__ );
#else
#define LOGI( fmt,... ) fprintf( stdout,fmt"\n",__VA_ARGS__ )
#define LOGE( fmt,... ) fprintf( stderr,fmt"\n",__VA_ARGS__ )
#define LOGD( fmt,... ) fprintf( stderr,fmt"\n",__VA_ARGS__ )
#endif
#endif

#endif
