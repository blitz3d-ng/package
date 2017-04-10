
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

BBTimer * BBCALL bbCreateTimer( int hertz );
int BBCALL bbWaitTimer( BBTimer *t );
void BBCALL bbFreeTimer( BBTimer *t );

#endif
