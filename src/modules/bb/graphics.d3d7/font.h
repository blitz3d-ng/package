#ifndef BB_BLITZ2D_D3D7_FONT_H
#define BB_BLITZ2D_D3D7_FONT_H

#include <bb/graphics/graphics.h>

class BBCanvas;
class BBGraphics;

class D3D7Font : public BBFont{
public:
	D3D7Font(
		BBGraphics *graphics,BBCanvas *canvas,
		int width,int height,int begin_char,int end_char,int def_char,
		int *offs,int *widths );
	~D3D7Font();

	int charWidth( int c )const;
	void render( BBCanvas *dest,unsigned color_argb,int x,int y,const std::string &t );

private:
	BBGraphics *graphics;
	BBCanvas *canvas,*t_canvas;
	int width,height,begin_char,end_char,def_char;
	int *offs,*widths;

	/***** GX INTERFACE *****/
public:
	enum{
		FONT_BOLD=1,
		FONT_ITALIC=2,
		FONT_UNDERLINE=4
	};

	//ACCESSORS
	int getWidth()const;							//width of widest char
	int getHeight()const;							//height of font
	int getWidth( const std::string &text )const;	//width of string
	bool isPrintable( int chr )const;				//printable char?
};

#endif
