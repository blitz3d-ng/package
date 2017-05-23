#ifndef BB_TIMER_COMMANDS_H
#define BB_TIMER_COMMANDS_H

#include <bb/blitz/module.h>

#ifdef __cplusplus
extern "C" {
#else
typedef void BBTimer;
#endif

BBTimer * BBCALL bbCreateTimer( int hertz );
int BBCALL bbWaitTimer( BBTimer *t );
void BBCALL bbFreeTimer( BBTimer *t );

#ifdef __cplusplus
}
#endif

#endif
