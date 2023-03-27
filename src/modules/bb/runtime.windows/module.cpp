
#include <bb/blitz/module.h>
#include "gxruntime.h"

BBRuntime *bbCreateD3D7Runtime(){
	return gxRuntime::openRuntime( GetModuleHandle(0) );
}

BBMODULE_EMPTY( runtime_windows );
