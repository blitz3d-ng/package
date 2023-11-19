#ifndef BB_PIXMAP_H
#define BB_PIXMAP_H

enum{
	PF_UNKNOWN=0,
	PF_I8,
	PF_RGB,
	PF_RGBA,
};

struct BBPixmap{
	int format;
	int width,height,depth,pitch,bpp;
	bool trans;
	unsigned char *bits;

#ifdef __cplusplus
	BBPixmap();
	~BBPixmap();

	int getWidth();
	int getHeight();

	int read( int x,int y );

	void mask( int r,int g,int b );
	void buildAlpha( bool whiten );

	void flipVertically();
	void swapBytes0and2();
#endif
};

#ifdef __cplusplus
#include <string>
BBPixmap *bbLoadPixmap( const std::string &file );
#endif

#endif
