#ifndef BB_GRAPHICS_GL_H
#define BB_GRAPHICS_GL_H

#include <bb/graphics/graphics.h>

#ifdef BB_DESKTOP
#include <GL/glew.h>
#endif

#if defined(BB_DESKTOP) || defined(BB_EMSCRIPTEN)
	#ifdef BB_MACOS
		#include <OpenGL/gl.h>
		#include <OpenGL/glext.h>
	#else
		#define GL_GLEXT_PROTOTYPES
		#include <GL/gl.h>
		// #include <GL/glext.h>
	#endif
#else
	#ifdef BB_IOS
		#include <OpenGLES/ES3/gl.h>
	#endif

	#ifdef __BIONIC__
		#include <GLES3/gl3.h>
	#endif
#endif

GLuint _bbGLCompileProgram( const std::string &name,const std::string &src );


// #ifdef NDEBUG
// #define GL( func ) func;
// #else
void bbGLGraphicsCheckErrors( const char *file, int line );
#define GL( func ) func; bbGLGraphicsCheckErrors( __FILE__,__LINE__ );
// #endif

#endif
