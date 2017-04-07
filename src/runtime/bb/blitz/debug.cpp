
#include "debug.h"

#include "../../gxruntime/gxruntime.h"
extern gxRuntime *gx_runtime;

void BBCALL bbDebugError( const char *e ){
  gx_runtime->debugError( e );
}
