
/* Win32 runtime dynamic link lib */

#ifndef BBRUNTIME_DLL_H
#define BBRUNTIME_DLL_H

#ifdef BB_WINDOWS
#include <windows.h>
#else
#define HINSTANCE void*
#endif

#include "../../../stdutil/stdutil.h"

class Debugger;

class Runtime{
public:
	virtual int version();
	virtual const char *nextSym();
	virtual bb_int_t symValue( const char *sym );
	virtual void startup( HINSTANCE hinst );
	virtual void shutdown();
	virtual void checkmem( std::streambuf *buf );

	virtual void execute( void (*pc)(),const char *args,Debugger *dbg );
};

extern "C" BBDECL Runtime * CDECL runtimeGetRuntime();

#endif
