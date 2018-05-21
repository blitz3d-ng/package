
#include <bb/blitz/module.h>
#include "gxruntime.h"

BBRuntime *bbCreateD3D7Runtime(){
	return gxRuntime::openRuntime( GetModuleHandle(0) );
}

void bbCloseRuntime( BBRuntime *rt ){
	return gxRuntime::closeRuntime( (gxRuntime*)rt );
}

BBMODULE_CREATE( runtime_windows ){
  return true;
}

BBMODULE_DESTROY( runtime_windows ){
  return true;
}
