#ifndef BB_BLITZ2D_MOVIE_H
#define BB_BLITZ2D_MOVIE_H

class BBMovie{
public:
  virtual bool draw( BBCanvas *dest,int x,int y,int w,int h )=0;

  virtual bool isPlaying()const=0;
  virtual int getWidth()const=0;
  virtual int getHeight()const=0;
};

#endif
