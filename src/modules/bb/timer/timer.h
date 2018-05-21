
#ifndef BBTIMER_H
#define BBTIMER_H

#include <bb/blitz/blitz.h>

class BBTimer{
public:
	virtual ~BBTimer();
	/***** GX INTERFACE *****/
public:
	virtual int wait()=0;
};

BBTimer * BBCALL _bbCreateTimer( int hertz );

#include "commands.h"

#endif
