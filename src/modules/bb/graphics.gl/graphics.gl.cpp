#include "../stdutil/stdutil.h"
#include "graphics.gl.h"

#include <syslog.h>

static
const char * GlErrorString( GLenum error ) {
	switch ( error ) {
  case GL_NO_ERROR:                      return "GL_NO_ERROR";
  case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
  case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
  case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
  case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
  case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
  default: return "unknown";
	}
}

const char * GlFrameBufferStatusString( GLenum status ) {
	switch ( status ) {
	// case GL_FRAMEBUFFER_UNDEFINED:                     return "GL_FRAMEBUFFER_UNDEFINED";
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:         return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
	case GL_FRAMEBUFFER_UNSUPPORTED:                   return "GL_FRAMEBUFFER_UNSUPPORTED";
	// case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:        return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
	default:                                           return "unknown";
	}
}

void bbGLGraphicsCheckErrors( const char *file, int line ){
	for( int i=0;i<10;i++ ){
		const GLenum error=glGetError();
		if( error==GL_NO_ERROR ){
			break;
		}
		syslog( LOG_WARNING,"GL error on %s:%d: %s", file, line, GlErrorString( error ) );
	}
}

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
