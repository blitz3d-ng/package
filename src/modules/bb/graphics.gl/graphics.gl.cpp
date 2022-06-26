#include "../stdutil/stdutil.h"
#include "graphics.gl.h"

BBMODULE_CREATE( graphics_gl ){
  #if defined(BB_WINDOWS) || defined(BB_LINUX) 
    return glewInit()==GLEW_OK;
  #else
    return true;
  #endif
}

BBMODULE_DESTROY( graphics_gl ){
  return true;
}
