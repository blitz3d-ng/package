
#include <bb/blitz/module.h>
#include "gxruntime.h"

BBRuntime *bbCreateD3D7Runtime(){
	return gxRuntime::openRuntime( GetModuleHandle(0) );
}

BBMODULE_CREATE( runtime_windows ){
  return true;
}

BBMODULE_DESTROY( runtime_windows ){
  return true;
}
