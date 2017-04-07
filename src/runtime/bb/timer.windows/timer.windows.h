
#ifndef GXTIMER_H
#define GXTIMER_H

#include <windows.h>
#include "../timer/timer.h"

class WindowsTimer : public BBTimer{
public:
	WindowsTimer( int hertz );
	~WindowsTimer();

	static void CALLBACK timerCallback( UINT id,UINT msg,DWORD user,DWORD dw1,DWORD dw2 );

private:
	HANDLE event;
	MMRESULT timerID;
	int ticks_put,ticks_get;

	/***** GX INTERFACE *****/
public:
	int wait();
};

#endif
