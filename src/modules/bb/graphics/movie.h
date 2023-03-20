#ifndef BB_BLITZ2D_MOVIE_H
#define BB_BLITZ2D_MOVIE_H

#include <bb/blitz/blitz.h>
#include <bb/graphics/canvas.h>

class BBMovie{
public:
  virtual ~BBMovie();

  virtual bool draw( BBCanvas *dest,int x,int y,int w,int h )=0;

  virtual bool isPlaying()const=0;
  virtual int getWidth()const=0;
  virtual int getHeight()const=0;
};

#endif
