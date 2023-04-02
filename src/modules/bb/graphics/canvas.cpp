#include "canvas.h"

BBCanvas::~BBCanvas(){
}

void BBCanvas::getLogicalViewport( int *x,int *y,int *w,int *h )const{
	float sx,sy;
	getScale( &sx,&sy );
	getViewport( x,y,w,h );
	*x/=sx;*y/=sy;*w/=sx;*h/=sy;
}
