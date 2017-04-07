#ifndef B2DGRAPHICS_H
#define B2DGRAPHICS_H

#include <bb/graphics/graphics.h>
#include "font.h"

class gxMovie;

class B2DGraphics : public BBGraphics{
public:
  virtual gxMovie *openMovie( const std::string &file,int flags )=0;
  virtual gxMovie *verifyMovie( gxMovie *movie )=0;
  virtual void closeMovie( gxMovie *movie )=0;

  virtual BBFont *loadFont( const std::string &font,int height,int flags )=0;
  virtual BBFont *verifyFont( BBFont *font )=0;
  virtual void freeFont( BBFont *font )=0;
};

#endif
