#include "egl.h"
#include <bb/blitz/app.h>

static const char* _bbESErrorString(const EGLint error) {
	switch (error) {
	case EGL_SUCCESS:
		return "EGL_SUCCESS";
	case EGL_NOT_INITIALIZED:
		return "EGL_NOT_INITIALIZED";
	case EGL_BAD_ACCESS:
		return "EGL_BAD_ACCESS";
	case EGL_BAD_ALLOC:
		return "EGL_BAD_ALLOC";
	case EGL_BAD_ATTRIBUTE:
		return "EGL_BAD_ATTRIBUTE";
	case EGL_BAD_CONTEXT:
		return "EGL_BAD_CONTEXT";
	case EGL_BAD_CONFIG:
		return "EGL_BAD_CONFIG";
	case EGL_BAD_CURRENT_SURFACE:
		return "EGL_BAD_CURRENT_SURFACE";
	case EGL_BAD_DISPLAY:
		return "EGL_BAD_DISPLAY";
	case EGL_BAD_SURFACE:
		return "EGL_BAD_SURFACE";
	case EGL_BAD_MATCH:
		return "EGL_BAD_MATCH";
	case EGL_BAD_PARAMETER:
		return "EGL_BAD_PARAMETER";
	case EGL_BAD_NATIVE_PIXMAP:
		return "EGL_BAD_NATIVE_PIXMAP";
	case EGL_BAD_NATIVE_WINDOW:
		return "EGL_BAD_NATIVE_WINDOW";
	case EGL_CONTEXT_LOST:
		return "EGL_CONTEXT_LOST";
	default:
		return "unknown";
	}
}

BBContextES::BBContextES(){
	clear();
}

BBContextES::~BBContextES(){
	destroy();
	clear();
}

void BBContextES::clear(){
	major_version=0;
	minor_version=0;
	display=0;
	config=0;
	tiny_surface=EGL_NO_SURFACE;
	main_surface=EGL_NO_SURFACE;
	context=EGL_NO_CONTEXT;
}

void BBContextES::create(BBContextES *share){
	if (display != 0) {
		return;
	}

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, &major_version, &minor_version);

	const int MAX_CONFIGS = 1024;
	EGLConfig configs[MAX_CONFIGS];
	EGLint numConfigs = 0;
	if (eglGetConfigs(display, configs, MAX_CONFIGS, &numConfigs) == EGL_FALSE) {
		return;
	}

	const EGLint configAttribs[] = {
		EGL_RED_SIZE,     8,
		EGL_GREEN_SIZE,   8,
		EGL_BLUE_SIZE,    8,
		EGL_ALPHA_SIZE,   8,
		EGL_DEPTH_SIZE,   0,
		EGL_STENCIL_SIZE, 0,
		EGL_SAMPLES,      0,
		EGL_NONE
	};
	config = 0;
	for (int i = 0; i < numConfigs; i++) {
		EGLint value = 0;

		eglGetConfigAttrib(display, configs[i], EGL_RENDERABLE_TYPE, &value);
		if ((value & EGL_OPENGL_ES3_BIT_KHR) != EGL_OPENGL_ES3_BIT_KHR) {
			continue;
		}

		eglGetConfigAttrib(display, configs[i], EGL_SURFACE_TYPE, &value);
		if ((value & (EGL_WINDOW_BIT | EGL_PBUFFER_BIT)) != (EGL_WINDOW_BIT | EGL_PBUFFER_BIT)) {
			continue;
		}

		int j = 0;
		for (; configAttribs[j] != EGL_NONE; j += 2) {
			eglGetConfigAttrib(display, configs[i], configAttribs[j], &value);
			if (value != configAttribs[j + 1]) {
				break;
			}
		}
		if (configAttribs[j] == EGL_NONE) {
			config = configs[i];
			break;
		}
	}

	if( config==0 ){
		return;
	}

	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
	context=eglCreateContext( display,config,(share!=NULL)?share->context:EGL_NO_CONTEXT,contextAttribs );
	if (context == EGL_NO_CONTEXT) {
		return;
	}

	const EGLint surfaceAttribs[] = {EGL_WIDTH, 16, EGL_HEIGHT, 16, EGL_NONE};
	tiny_surface = eglCreatePbufferSurface(display, config, surfaceAttribs);
	if (tiny_surface == EGL_NO_SURFACE) {
		eglDestroyContext(display, context);
		context=EGL_NO_CONTEXT;
		return;
	}

	if( eglMakeCurrent(display, tiny_surface, tiny_surface, context)==EGL_FALSE ){
		eglDestroySurface(display, tiny_surface);
		eglDestroyContext(display, context);
		context = EGL_NO_CONTEXT;
		return;
	}
}

void BBContextES::destroy(){
	if( display!=0 ){
		eglMakeCurrent( display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT );
	}

	if( context!=EGL_NO_CONTEXT ){
		eglDestroyContext( display,context );
		context=EGL_NO_CONTEXT;
	}

	if( tiny_surface!=EGL_NO_SURFACE ){
		eglDestroySurface( display,tiny_surface );
		tiny_surface=EGL_NO_SURFACE;
	}

	if( display!=0 ){
		eglTerminate(display);
		display=0;
	}
}
