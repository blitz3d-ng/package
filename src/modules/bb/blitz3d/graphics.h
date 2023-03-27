#ifndef B3DGRAPHICS_H
#define B3DGRAPHICS_H

#include "mesh.h"
#include "scene.h"

class B3DGraphics{
protected:
  std::set<BBScene*> scene_set;

public:
  ~B3DGraphics();

  virtual BBScene *createScene( int w,int h,float d,int flags )=0;
  BBScene *verifyScene( BBScene *scene );
  void freeScene( BBScene *scene );
};

extern B3DGraphics *bbSceneDriver;

#endif
