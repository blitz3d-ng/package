#ifndef BBRUNTIME_DLL_H
#define BBRUNTIME_DLL_H

#include <bb/blitz/blitz.h>
#include "../../stdutil/stdutil.h"

#include <string>
#include <map>
#include <set>

class Debugger;

class Runtime{
public:
	std::string path;

	virtual int version();
	virtual const char *nextSym();
	virtual bb_int_t symValue( const char *sym );
	virtual void loadSyms( std::map<const char*,void*> &syms );
	virtual void startup();
	virtual void shutdown();
	virtual void checkmem( std::streambuf *buf );

	virtual int execute( void (*pc)(),const char *args,Debugger *dbg );
};

struct BBSymbol{
public:
	std::string ident,sym;
	bb_int_t value;
};

struct BBModule{
public:
	std::string id,ident;
	std::vector<std::string> deps,libs,system_libs;
	std::vector<BBSymbol> syms;
	bool used=false,imported=false;
};

struct BBRuntimeDylib{
public:
	std::string id,entry;
	std::map<std::string,BBModule> modules;
	std::vector<std::string> order;
	std::map<std::string,std::string> syms;
};

extern "C" BBDECL Runtime * CDECL runtimeGetRuntime();

#endif
