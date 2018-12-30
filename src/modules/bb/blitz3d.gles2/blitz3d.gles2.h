#ifndef BB_BLITZ3D_GLES2_H
#define BB_BLITZ3D_GLES2_H

#include <bb/blitz3d/blitz3d.h>
#include <bb/blitz3d/graphics.h>

class GLES2B3DGraphics : public B3DGraphics{
public:
	BBScene *createScene( int w,int h,int flags );
	BBScene *verifyScene( BBScene *scene );
	void freeScene( BBScene *scene );
};

#endif
