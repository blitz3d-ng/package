#ifndef BB_BLITZ3D_GL_H
#define BB_BLITZ3D_GL_H

#include <bb/blitz3d/blitz3d.h>
#include <bb/blitz3d/graphics.h>

class GLB3DGraphics : public B3DGraphics{
public:
	BBScene *createScene( int flags );
	BBScene *verifyScene( BBScene *scene );
	void freeScene( BBScene *scene );
};

#endif
