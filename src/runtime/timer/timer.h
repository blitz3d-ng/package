
#ifndef BBTIMER_H
#define BBTIMER_H

class BBTimer{
public:
	// BBTimer( int hertz );
	// ~BBTimer();
	/***** GX INTERFACE *****/
public:
	virtual int wait()=0;
};

#endif
