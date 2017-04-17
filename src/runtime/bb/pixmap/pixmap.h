#ifndef BB_PIXMAP_H
#define BB_PIXMAP_H

#include <string>

struct BBPixmap{
  int width,height,depth,pitch;
  void *bits;

  ~BBPixmap();
};

BBPixmap *bbLoadPixmap( const std::string &file );

#endif
