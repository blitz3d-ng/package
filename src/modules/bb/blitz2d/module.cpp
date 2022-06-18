
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

void BBCALL bbOrigin( bb_int_t x,bb_int_t y ){
	gx_canvas->setOrigin( x,y );
}

void BBCALL bbViewport( bb_int_t x,bb_int_t y,bb_int_t w,bb_int_t h ){
	gx_canvas->setViewport( x,y,w,h );
}

void BBCALL bbColor( bb_int_t r,bb_int_t g,bb_int_t b ){
	gx_canvas->setColor( curr_color=(r<<16)|(g<<8)|b );
}

void BBCALL bbGetColor( bb_int_t x,bb_int_t y ){
	gx_canvas->setColor( curr_color=gx_canvas->getPixel( x,y ) );
}

bb_int_t BBCALL bbColorRed(){
	return (gx_canvas->getColor()>>16)&0xff;
}

bb_int_t BBCALL bbColorGreen(){
	return (gx_canvas->getColor()>>8)&0xff;
}

bb_int_t BBCALL bbColorBlue(){
	return gx_canvas->getColor()&0xff;
}

void BBCALL bbClsColor( bb_int_t r,bb_int_t g,bb_int_t b ){
	gx_canvas->setClsColor( curr_clsColor=(r<<16)|(g<<8)|b );
}

void BBCALL bbCls(){
	gx_canvas->cls();
}

void BBCALL bbPlot( bb_int_t x,bb_int_t y ){
	gx_canvas->plot( x,y );
}

void BBCALL bbLine( bb_int_t x1,bb_int_t y1,bb_int_t x2,bb_int_t y2 ){
	gx_canvas->line( x1,y1,x2,y2 );
}

void BBCALL bbRect( bb_int_t x,bb_int_t y,bb_int_t w,bb_int_t h,bb_int_t solid ){
	gx_canvas->rect( x,y,w,h,solid ? true : false );
}

void BBCALL bbOval( bb_int_t x,bb_int_t y,bb_int_t w,bb_int_t h,bb_int_t solid ){
	gx_canvas->oval( x,y,w,h,solid ? true : false );
}

void BBCALL bbText( bb_int_t x,bb_int_t y,BBStr *str,bb_int_t centre_x,bb_int_t centre_y ){
	debugFont( curr_font );
	if( centre_x ) x-=curr_font->getWidth( *str )/2;
	if( centre_y ) y-=curr_font->getHeight()/2;
	gx_canvas->text( x,y,*str );
	delete str;
}

BBFont * BBCALL bbLoadFont( BBStr *name,bb_int_t height,bb_int_t bold,bb_int_t italic,bb_int_t underline ){
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

bb_int_t BBCALL bbFontWidth(){
	debugFont( curr_font );
	return curr_font->getWidth();
}

bb_int_t BBCALL bbFontHeight(){
	debugFont( curr_font );
	return curr_font->getHeight();
}

bb_int_t BBCALL bbStringWidth( BBStr *str ){
	debugFont( curr_font );
	string t=*str;delete str;
	return curr_font->getWidth( t );
}

bb_int_t BBCALL bbStringHeight( BBStr *str ){
	debugFont( curr_font );
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
