
#include "../../stdutil/stdutil.h"
#include "runtime.console.h"

class ConsoleRuntime : public BBRuntime{
public:
	// runtime

	void asyncStop(){}
	void asyncRun(){}
	void asyncEnd(){}

	bool idle(){ return true; }

	void *window(){ return 0; }

	void moveMouse( int x,int y ){}
	void setPointerVisible( bool vis ){}
};

BBRuntime *bbCreateRuntime(){
	return d_new ConsoleRuntime();
}

BBMODULE_CREATE( runtime_console ){
	return true;
}

BBMODULE_DESTROY( runtime_console ){
	return true;
}
