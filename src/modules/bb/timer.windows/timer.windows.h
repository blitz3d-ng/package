
#ifndef GXTIMER_H
#define GXTIMER_H

#include <windows.h>
#include "../timer/timer.h"

class WindowsTimer : public BBTimer{
public:
	WindowsTimer( int hertz );
	~WindowsTimer();

	static void CALLBACK timerCallback( UINT id,UINT msg,DWORD_PTR user,DWORD_PTR dw1,DWORD_PTR dw2 );

private:
	HANDLE event;
	MMRESULT timerID;
	int ticks_put,ticks_get;

	/***** GX INTERFACE *****/
public:
	int wait();
};

#endif
