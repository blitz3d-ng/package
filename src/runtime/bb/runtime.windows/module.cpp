
#include <bb/blitz/module.h>
#include "gxruntime.h"

BBRuntime *bbCreateRuntime(){
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
