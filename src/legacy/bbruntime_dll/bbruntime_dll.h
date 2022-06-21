#ifndef BBRUNTIME_DLL_H
#define BBRUNTIME_DLL_H

#include <bb/blitz/blitz.h>
#include "../../../stdutil/stdutil.h"

#include <string>

class Debugger;

class Runtime{
public:
	std::string path;

	virtual int version();
	virtual const char *nextSym();
	virtual bb_int_t symValue( const char *sym );
	virtual void startup();
	virtual void shutdown();
	virtual void checkmem( std::streambuf *buf );

	virtual void execute( void (*pc)(),const char *args,Debugger *dbg );
};

extern "C" BBDECL Runtime * CDECL runtimeGetRuntime();

#endif
