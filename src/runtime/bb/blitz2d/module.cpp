
#include <bb/blitz/module.h>
#include "blitz2d.h"

using namespace std;

#define b2d_graphics ((B2DGraphics*)gx_graphics)

extern FT_Library ft;

/*static*/ BBFont *curr_font;
/*static*/ unsigned curr_color;
/*static*/ unsigned curr_clsColor;


static inline void debugFont( BBFont *f ){
	if( bb_env.debug ){
		if( !b2d_graphics->verifyFont( f ) ) RTEX( "Font does not exist" );
	}
}

void BBCALL bbOrigin( int x,int y ){
	gx_canvas->setOrigin( x,y );
}

void BBCALL bbViewport( int x,int y,int w,int h ){
	gx_canvas->setViewport( x,y,w,h );
}

void BBCALL bbColor( int r,int g,int b ){
	gx_canvas->setColor( curr_color=(r<<16)|(g<<8)|b );
}

void BBCALL bbGetColor( int x,int y ){
	gx_canvas->setColor( curr_color=gx_canvas->getPixel( x,y ) );
}

int BBCALL bbColorRed(){
	return (gx_canvas->getColor()>>16)&0xff;
}

int BBCALL bbColorGreen(){
	return (gx_canvas->getColor()>>8)&0xff;
}

int BBCALL bbColorBlue(){
	return gx_canvas->getColor()&0xff;
}

void BBCALL bbClsColor( int r,int g,int b ){
	gx_canvas->setClsColor( curr_clsColor=(r<<16)|(g<<8)|b );
}

void BBCALL bbCls(){
	gx_canvas->cls();
}

void BBCALL bbPlot( int x,int y ){
	gx_canvas->plot( x,y );
}

void BBCALL bbLine( int x1,int y1,int x2,int y2 ){
	gx_canvas->line( x1,y1,x2,y2 );
}

void BBCALL bbRect( int x,int y,int w,int h,int solid ){
	gx_canvas->rect( x,y,w,h,solid ? true : false );
}

void BBCALL bbOval( int x,int y,int w,int h,int solid ){
	gx_canvas->oval( x,y,w,h,solid ? true : false );
}

void BBCALL bbText( int x,int y,BBStr *str,int centre_x,int centre_y ){
	if( centre_x ) x-=curr_font->getWidth( *str )/2;
	if( centre_y ) y-=curr_font->getHeight()/2;
	gx_canvas->text( x,y,*str );
	delete str;
}

BBFont * BBCALL bbLoadFont( BBStr *name,int height,int bold,int italic,int underline ){
	int flags=
		(bold ? BBFont::FONT_BOLD : 0 ) |
		(italic ? BBFont::FONT_ITALIC : 0 ) |
		(underline ? BBFont::FONT_UNDERLINE : 0 );
	BBFont *font=b2d_graphics->loadFont( *name,height,flags );
	delete name;
	return font;
}

void BBCALL bbFreeFont( BBFont *f ){
	debugFont( f );
	if( f==curr_font ) bbSetFont( gx_graphics->getDefaultFont() );
	b2d_graphics->freeFont( f );
}

void BBCALL bbSetFont( BBFont *f ){
	debugFont( f );
	gx_canvas->setFont( curr_font=f );
}

int BBCALL bbFontWidth(){
	return curr_font->getWidth();
}

int BBCALL bbFontHeight(){
	return curr_font->getHeight();
}

int BBCALL bbStringWidth( BBStr *str ){
	string t=*str;delete str;
	return curr_font->getWidth( t );
}

int BBCALL bbStringHeight( BBStr *str ){
	delete str;
	return curr_font->getHeight();
}

BBMODULE_CREATE( blitz2d ){
	if( FT_Init_FreeType(&ft) ) return false;

  return true;
}

BBMODULE_DESTROY( blitz2d ){
  return true;
}
