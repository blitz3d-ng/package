
#include "../stdutil/stdutil.h"
#include "runtime.nx.h"

#include <switch.h>

void bbSystemPollEvent(){

}

class NXRuntime : public BBRuntime{
public:
	// runtime

	void asyncStop(){}
	void asyncRun(){}
	void asyncEnd(){}

	bool idle(){ bbSystemPollEvent();return true; }

	void *window(){ return 0; }

	void moveMouse( int x,int y ){}
	void setPointerVisible( bool vis ){}
};

BBRuntime *bbCreateOpenGLRuntime(){
	return d_new NXRuntime();
}

BBMODULE_EMPTY( runtime_nx );
