#include "bbruntime_dll.h"
#include <bb/runtime/runtime.h>
#include <bb/stub/stub.h>

#include <map>
using namespace std;

static map<const char*,void*> syms;
map<const char*,void*>::iterator sym_it;

static void linkSym( const char *sym,void *pc ){
	syms[sym]=pc;
}

int Runtime::version(){
	return VERSION;
}

const char *Runtime::nextSym(){
	if( !syms.size() ){
		bbruntime_link( linkSym );
		sym_it=syms.begin();
	}
	if( sym_it==syms.end() ){
		syms.clear();return 0;
	}
	return (sym_it++)->first;
}

bb_int_t Runtime::symValue( const char *sym ){
	map<const char*,void*>::iterator it=syms.find( sym );
	if( it!=syms.end() ) return (bb_int_t)it->second;
	return -1;
}

void Runtime::startup(){
}

void Runtime::shutdown(){
	trackmem( false );
	syms.clear();
}

#ifndef BB_WINDOWS
void Runtime::execute( void (*pc)(),const char *args,Debugger *dbg ){
}
#endif

void Runtime::checkmem( streambuf *buf ){
	ostream out( buf );
	::checkmem( out );
}

#ifndef BB_MSVC
__attribute__((used))
#endif
Runtime * CDECL runtimeGetRuntime(){
	static Runtime runtime;
	return &runtime;
}
