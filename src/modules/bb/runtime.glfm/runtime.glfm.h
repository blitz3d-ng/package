#ifndef BB_RUNTIME_GLFM_H
#define BB_RUNTIME_GLFM_H

#include <bb/runtime/runtime.h>
#include <bb/graphics/graphics.h>
#include <bb/blitz2d.gles2/blitz2d.gles2.h>
#include <bb/blitz3d.gles2/blitz3d.gles2.h>
#include <glfm.h>

// class GLFMGraphics;

class GLFMRuntime : public GLES2B3DGraphics, public BBRuntime, public BBContextDriver{
private:
	GLFMDisplay *display;

// 	static void _onResize( GLFWwindow *wnd,int w,int h );
// 	static void _onClose( GLFWwindow *w );
// 	static void _onMouseMove( GLFWwindow *w,double x,double y );
// 	static void _onMouseButton( GLFWwindow *w,int button,int action,int mods );
// 	static void _onKeyChange( GLFWwindow *w,int key,int scancode,int action,int mods );

// 	void resize( int width,int height );
public:
	GLFMRuntime( GLFMDisplay *display );
	~GLFMRuntime();

	// context driver
	int numGraphicsDrivers();
	void graphicsDriverInfo( int driver,std::string *name,int *c );
	int numGraphicsModes( int driver );
	void graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c );
	void windowedModeInfo( int *c );

	BBGraphics *openGraphics( int w,int h,int d,int driver,int flags );
	void closeGraphics( BBGraphics *graphics );
	bool graphicsLost();

	void flip( bool vwait );
//
// 	// runtime
	void asyncStop();
	void asyncRun();
	void asyncEnd();

	bool idle();

	void *window();

	void moveMouse( int x,int y );
	void setPointerVisible( bool vis );
};

#endif
