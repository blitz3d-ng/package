#ifndef BB_GRAPHICS_GL_EGL_H
#define BB_GRAPHICS_GL_EGL_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

class BBContextES {
public:
	EGLint major_version,minor_version;
	EGLDisplay display;
	EGLConfig config;
	EGLSurface main_surface,tiny_surface;
	EGLContext context;

	BBContextES();
	~BBContextES();

	void clear();
	void create(BBContextES *share);
	void destroy();
};

#endif
