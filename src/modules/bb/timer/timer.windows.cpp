
#include "timer.h"
#include <windows.h>

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

WindowsTimer::WindowsTimer( int hertz ):
ticks_get(0),ticks_put(0){
	event=CreateEvent( 0,false,false,0 );
	timerID=timeSetEvent( 1000/hertz,0,timerCallback,(DWORD_PTR )this,TIME_PERIODIC );
}

WindowsTimer::~WindowsTimer(){
	timeKillEvent( timerID );
	CloseHandle( event );
}

void CALLBACK WindowsTimer::timerCallback( UINT id,UINT msg,DWORD_PTR user,DWORD_PTR dw1,DWORD_PTR dw2 ){
	WindowsTimer *t=(WindowsTimer*)user;
	++t->ticks_put;
	SetEvent( t->event );
}

int WindowsTimer::wait(){
	for(;;){
		if( WaitForSingleObject( event,1000 )==WAIT_OBJECT_0 ) break;
	}
	int n=ticks_put-ticks_get;
	ticks_get+=n;
	return n;
}

BBTimer * BBCALL _osCreateTimer( int hertz ){
	return new WindowsTimer( hertz );
}
