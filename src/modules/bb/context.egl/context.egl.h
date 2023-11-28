#ifndef BB_CONTEXT_EGL_H
#define BB_CONTEXT_EGL_H

#include <bb/graphics.gl/graphics.gl.h>
#include <bb/blitz3d.gl/blitz3d.gl.h>
#include "egl.h"

class EGLContextDriver:public GLB3DGraphics,public BBContextDriver{
private:
public:
	BBContextES egl;

	EGLContextDriver();
	~EGLContextDriver();

	int numGraphicsDrivers();
	void graphicsDriverInfo( int driver,std::string *name,int *c );
	int numGraphicsModes( int driver );
	void graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c );
	void windowedModeInfo( int *c );

	BBGraphics *openGraphics( int w,int h,int d,int driver,int flags );
	void closeGraphics( BBGraphics *graphics );
	bool graphicsLost();

	void flip( bool vwait );
};

#endif
