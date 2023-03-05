
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <bb/blitz/debug.h>

#ifdef WIN32
#define CDECL __cdecl
#else
#define CDECL
#endif

extern "C" BBDECL Debugger * CDECL debuggerGetDebugger( void *mod,void *env );

#endif
