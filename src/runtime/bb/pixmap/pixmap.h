#ifndef BB_PIXMAP_H
#define BB_PIXMAP_H

#include <string>

enum{
	PF_UNKNOWN=0,
	PF_RGB,
	PF_RGBA,
};

struct BBPixmap{
	int format;
  int width,height,depth,pitch,bpp;
  unsigned char *bits;

  BBPixmap();
  ~BBPixmap();

	void mask( int r,int g,int b );
	void buildAlpha( bool whiten );
};

BBPixmap *bbLoadPixmap( const std::string &file );

#endif
