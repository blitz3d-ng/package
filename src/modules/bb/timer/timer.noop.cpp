#include <bb/timer/timer.h>

class NoopTimer : public BBTimer{
protected:
	int hertz;
public:
	NoopTimer( int h ):hertz(h){}
	~NoopTimer(){}

	int wait(){
		return 0; // FIXME: implement this properly.
	}
};

BBTimer * BBCALL _osCreateTimer( int hertz ){
	return new NoopTimer( hertz );
}
