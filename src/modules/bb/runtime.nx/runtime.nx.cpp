
#include "../stdutil/stdutil.h"
#include "runtime.nx.h"

#include <switch.h>

#include <bb/input/input.h>
#include <bb/graphics/graphics.h>
#include <math.h>

void bbSystemPollEvent(){
	appletMainLoop();
}

void BBCALL bbInitConsole(){
	 consoleInit( 0 );
}

void BBCALL bbUpdateConsole(){
	consoleUpdate( 0 );
}

class NXRuntime : public BBRuntime{
public:
	~NXRuntime(){
		consoleExit( 0 );
	}

	void afterCreate(){
		// consoleInit( 0 );

		romfsInit();

		// BBContextDriver::change( "gl" );
		// bbDefaultGraphics();
	}

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
