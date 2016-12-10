#ifndef B3DGRAPHICS_H
#define B3DGRAPHICS_H

#include "../blitz2d/graphics.h"
#include "mesh.h"
#include "scene.h"

class B3DGraphics : public B2DGraphics{
public:
  virtual BBMesh *createMesh( int max_verts,int max_tris,int flags )=0;
  virtual BBMesh *verifyMesh( BBMesh *mesh )=0;
  virtual void freeMesh( BBMesh *mesh )=0;

  virtual BBScene *createScene( int flags )=0;
  virtual BBScene *verifyScene( BBScene *scene )=0;
  virtual void freeScene( BBScene *scene )=0;
};

#endif
