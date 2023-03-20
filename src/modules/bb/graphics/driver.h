#ifndef B2DGRAPHICS_H
#define B2DGRAPHICS_H

#include "graphics.h"
#include "movie.h"
#include "font.h"

class B2DGraphics : public BBGraphics{
protected:
  std::set<BBMovie*> movie_set;
  std::set<BBFont*> font_set;

public:
  ~B2DGraphics();

  virtual BBMovie *openMovie( const std::string &file,int flags )=0;
  BBMovie *verifyMovie( BBMovie *movie );
  void closeMovie( BBMovie *movie );

  virtual BBFont *loadFont( const std::string &font,int height,int flags );
  BBFont *verifyFont( BBFont *font );
  void freeFont( BBFont *font );
};

#endif
