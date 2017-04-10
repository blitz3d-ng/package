#ifndef BB_RUNTIME_RUNTIME_H
#define BB_RUNTIME_RUNTIME_H

#include <bb/hook/hook.h>

void *bbRuntimeWindow();
bool bbRuntimeIdle();

extern BBHook *bbRuntimeOnSuspend;
extern BBHook *bbRuntimeOnResume;

#endif
