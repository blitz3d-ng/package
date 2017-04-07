
#ifndef USERLIBS_H
#define USERLIBS_H

#include <bb/blitz/basic.h>

void	BBCALL	 _bbLoadLibs( char *p );

const char*	BBCALL _bbStrToCStr( BBStr *str );
BBStr*		BBCALL _bbCStrToStr( const char *str );

#endif
