
#ifndef BBTIMER_H
#define BBTIMER_H

#include <bb/blitz/blitz.h>

class BBTimer{
public:
	// BBTimer( int hertz );
	// ~BBTimer();
	/***** GX INTERFACE *****/
public:
	virtual int wait()=0;
};

BBTimer * BBCALL bbCreateTimer( int hertz );
int BBCALL bbWaitTimer( BBTimer *t );
void BBCALL bbFreeTimer( BBTimer *t );

#endif
