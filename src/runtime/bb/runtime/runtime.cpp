
#include "../../stdutil/stdutil.h"
#include <bb/blitz/blitz.h>
#include "runtime.h"

BBHook *bbRuntimeOnSuspend;
BBHook *bbRuntimeOnResume;

BBMODULE_CREATE( runtime ){
  bbRuntimeOnSuspend=d_new BBHook();
  bbRuntimeOnResume=d_new BBHook();
  return true;
}

BBMODULE_DESTROY( runtime ){
  delete bbRuntimeOnSuspend;
  delete bbRuntimeOnResume;
  return true;
}

BBMODULE_LINK( runtime ){
}
