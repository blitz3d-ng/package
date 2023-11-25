#ifndef BB_BLITZ2D_FONT_H
#define BB_BLITZ2D_FONT_H

#include <bb/pixmap/pixmap.h>

#include <ft2build.h>
#include <sys/_types/_int32_t.h>
#include FT_FREETYPE_H

#include <string>
#include <map>

class BBFont{
public:
	virtual ~BBFont();

	/***** GX INTERFACE *****/
public:
	enum{
		FONT_BOLD=1,
		FONT_ITALIC=2,
		FONT_UNDERLINE=4
	};

	//ACCESSORS
	virtual int getWidth()const=0;							//width of widest char
	virtual int getHeight()const=0;							//height of font
	virtual int getWidth( const std::string &text )const=0;	//width of string
	virtual bool isPrintable( int chr )const=0;				//printable char?
};

class BBImageFont : public BBFont{
public:
	struct Char{
		uint32_t index;
		int x,y,width,height;
		int bearing_x,bearing_y;
		int advance;
	};

	BBPixmap *atlas;
	mutable bool dirty;
	int baseline;
	float density;

private:
	FT_Face face;
	mutable std::map<uint32_t,Char> characters;

	BBImageFont( FT_Face f,int height,float density );

public:
	static BBImageFont *load( const std::string &name,int height,float density,int flags );

	bool loadChar( int32_t c )const;
	bool loadChars( const std::string &t )const;
	void rebuildAtlas();

	Char &getChar( uint32_t c );
	float getKerning( uint32_t l,uint32_t r );

	int getWidth()const;
	int getHeight()const;
	int getWidth( const std::string &text )const;
	bool isPrintable( int chr )const;
};

#endif
