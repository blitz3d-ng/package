#ifndef BB_GRAPHICS_GL_H
#define BB_GRAPHICS_GL_H

#include <bb/graphics/graphics.h>

#if defined(__linux__) || defined(_WIN32)
#include <GL/glew.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
// #include <GL/glext.h>
#endif

#endif
