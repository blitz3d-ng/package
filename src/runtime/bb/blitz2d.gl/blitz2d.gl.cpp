
#include "blitz2d.gl.h"

BBMODULE_CREATE( blitz2d_gl ){
#if defined(__linux__) || defined(_WIN32)
	return glewInit()==GLEW_OK;
#else
	return true;
#endif
}

BBMODULE_DESTROY( blitz2d_gl ){
	return true;
}
