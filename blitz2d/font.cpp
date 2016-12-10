
#include "font.h"
#include "../graphics/canvas.h"
#include "../graphics/graphics.h"

BBFont::BBFont( BBGraphics *g,BBCanvas *c,int w,int h,int b,int e,int d,int *os,int *ws ):
graphics(g),canvas(c),
width(w),height(h),begin_char(b),end_char(e),def_char(d),
offs(os),widths(ws){
	canvas->setMask( 0xffffff );
	t_canvas=graphics->createCanvas( graphics->getWidth(),height,0 );
}

BBFont::~BBFont(){
	delete[] offs;
	delete[] widths;
	graphics->freeCanvas( t_canvas );
	graphics->freeCanvas( canvas );
}

int BBFont::charWidth( int c )const{
	if( c<begin_char || c>=end_char ) c=def_char;
	return widths[c-begin_char];
}

void BBFont::render( BBCanvas *dest,unsigned color_argb,int x,int y,const std::string &t ){
	int width=getWidth( t );
	if( width>t_canvas->getWidth() ){
		graphics->freeCanvas( t_canvas );
		t_canvas=graphics->createCanvas( width,height,0 );
	}

	t_canvas->setColor( color_argb );
	if( !(t_canvas->getColor()&0xffffff) ) t_canvas->setColor( 0x10 );
	t_canvas->rect( 0,0,width,height,true );

	int t_x=0;
	for( int k=0;k<t.size();++k ){
		int c=t[k]&0xff;
		if( c<begin_char || c>=end_char ) c=def_char;
		c-=begin_char;
		int sx=(offs[c]>>16)&0xffff,sy=offs[c]&0xffff;
		t_canvas->blit( t_x,0,canvas,sx,sy,widths[c],height,false );
		t_x+=widths[c];
	}

	dest->blit( x,y,t_canvas,0,0,width,height,false );
}

int BBFont::getWidth()const{
	return width;
}

int BBFont::getHeight()const{
	return height;
}

int BBFont::getWidth( const std::string &t )const{
	int w=0;
	for( int k=0;k<t.size();++k ){
		int c=t[k]&0xff;
		if( c<begin_char || c>=end_char ) c=def_char;
		w+=widths[c-begin_char];
	}
	return w;
}

bool BBFont::isPrintable( int chr )const{
	return chr>=begin_char && chr<end_char;
}
