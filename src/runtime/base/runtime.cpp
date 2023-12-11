#include "bbruntime_dll.h"
#include <bb/runtime/runtime.h>
#include <bb/stub/stub.h>

#include <map>

static std::map<const char*,void*> syms;
static std::map<const char*,const char*> idents;
std::map<const char*,void*>::iterator sym_it;

static void linkSym( const char *sym,const char *ident,void *pc ){
	syms[sym]=pc;
	idents[sym]=ident;
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
	std::map<const char*,void*>::iterator it=syms.find( sym );
	if( it!=syms.end() ) return (bb_int_t)it->second;
	return -1;
}

const char *Runtime::symName( const char *sym ){
	std::map<const char*,const char*>::iterator it=idents.find( sym );
	if( it!=idents.end() ) return it->second;
	return "__not_found__";
}

void Runtime::loadSyms( std::map<const char*,void*> &_syms ){
	bbruntime_link( linkSym );

	std::copy( syms.begin(),syms.end(), std::inserter( _syms,_syms.end() ) );
	syms.clear();
}

void Runtime::startup(){
}

void Runtime::shutdown(){
	trackmem( false );
	syms.clear();
}

#ifndef BB_WINDOWS
int Runtime::execute( void (*pc)(),const char *args,Debugger *dbg ){
	return 0;
}
#endif

void Runtime::checkmem( std::streambuf *buf ){
	std::ostream out( buf );
	::checkmem( out );
}

BBDECL Runtime * CDECL runtimeGetRuntime(){
	static Runtime runtime;
	return &runtime;
}
