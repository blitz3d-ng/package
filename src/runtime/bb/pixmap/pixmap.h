#ifndef BB_PIXMAP_H
#define BB_PIXMAP_H

#include <string>

struct BBPixmap{
  int width,height,depth,pitch,bpp;
  unsigned char *bits;

  BBPixmap();
  ~BBPixmap();

	void mask( int r,int g,int b );
};

BBPixmap *bbLoadPixmap( const std::string &file );

#endif
