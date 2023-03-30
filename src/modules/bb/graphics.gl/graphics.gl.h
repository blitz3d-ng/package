#ifndef BB_GRAPHICS_GL_H
#define BB_GRAPHICS_GL_H

#include <bb/graphics/graphics.h>
#include <map>

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

	#if defined(BB_NDK) || defined(BB_NX)
		#include <GLES3/gl3.h>
	#endif
#endif

GLuint _bbGLCompileProgram( const std::string &name,const std::string &src );


// #ifdef NDEBUG
// #define GL( func ) func;
// #else
void bbGLGraphicsCheckErrors( const char *file, int line );
const char * bbGLFramebufferStatusString( GLenum status );
#define GL( func ) func; bbGLGraphicsCheckErrors( __FILE__,__LINE__ );
// #endif

struct ContextResources{
	unsigned int ubo;
	GLuint default_program;
	GLuint plot_buffer,plot_array;
	GLuint line_buffer,line_array;
	GLuint quad_buffer[2],quad_array[2];
	GLuint oval_buffer,oval_array;
	GLuint text_buffer,text_array;
	std::map<BBImageFont*,unsigned int> font_textures;
};

class GLGraphics:public BBGraphics{
protected:
	BBImageFont *def_font;

public:
	ContextResources res={ 0 }; // TODO: will make protected again

	BBFont *getDefaultFont()const;

	//OBJECTS
	BBCanvas *createCanvas( int width,int height,int flags );
	BBCanvas *loadCanvas( const std::string &file,int flags );

	// b2dgraphics
	BBMovie *openMovie( const std::string &file,int flags );
};

#include "canvas.h"

#endif
