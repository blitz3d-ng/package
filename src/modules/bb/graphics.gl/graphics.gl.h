#ifndef BB_GRAPHICS_GL_H
#define BB_GRAPHICS_GL_H

#include <bb/graphics/graphics.h>

#ifdef BB_DESKTOP
	#ifdef BB_MACOS
		#include <OpenGL/gl.h>
		#include <OpenGL/glext.h>
	#else
		#include <GL/glew.h>
		#define GL_GLEXT_PROTOTYPES
		#include <GL/gl.h>
		// #include <GL/glext.h>
	#endif
#else
	#ifdef BB_IOS
		#include <OpenGLES/ES2/gl.h>
	#endif

	#ifdef BB_ANDROID
		#include <GLES2/gl2.h>
	#endif
#endif


void bbGLGraphicsCheckErrors( const char *file, int line );

#ifdef NDEBUG
#define GL( func ) func;
#else
#define GL( func ) func; bbGLGraphicsCheckErrors( __FILE__,__LINE__ );
#endif

#endif
