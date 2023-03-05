#ifndef BB_TIMER_COMMANDS_H
#define BB_TIMER_COMMANDS_H

#include <bb/blitz/module.h>
#include <bb/timer/timer.h>

#ifdef __cplusplus
extern "C" {
#endif

// AUTOGENERATED. DO NOT EDIT.
// RUN `make` TO UPDATE.
BBTimer * BBCALL bbCreateTimer( bb_int_t hertz );
bb_int_t BBCALL bbWaitTimer( BBTimer *timer );
void BBCALL bbFreeTimer( BBTimer *timer );

#ifdef __cplusplus
}
#endif


#endif
