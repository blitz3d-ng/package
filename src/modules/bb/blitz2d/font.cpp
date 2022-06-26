
#include "../stdutil/stdutil.h"
#include <bb/system/system.h>
#include "font.h"
using namespace std;

FT_Library ft;
map<string,BBFontData> bbFontCache;

BBFont::~BBFont(){
}

BBImageFont::BBImageFont( FT_Face f,int height ):face(f),atlas(0){
	while( height ){
		if( !FT_Set_Pixel_Sizes( face,0,height ) ) break;
		height-=1;
	}
	baseline=(face->ascender+face->descender)>>6;
}

BBImageFont *BBImageFont::load( const string &name,int height,int flags ){
	BBFontData font;

	if( bbFontCache.count( name )==0 ){
		if( bbSystemDriver->lookupFontData( name,font ) ){
			bbFontCache.insert( make_pair( name,font ) );
		}else{
			return 0;
		}
	}else{
		font=bbFontCache[name];
	}

	FT_Face face;
	if( FT_New_Memory_Face( ft,font.data,font.size,0,&face ) ){
		return 0;
	}
	return d_new BBImageFont( face,height );
}

bool BBImageFont::loadChars( const string &t )const{
	for( int i=0;i<t.length();i++ ){
		if( !characters.count(t[i]) ){
			Char chr;
			chr.index=FT_Get_Char_Index( face,t[i] );
			FT_Load_Glyph( face,chr.index,FT_LOAD_RENDER );

			chr.width=face->glyph->bitmap.width;
			chr.height=face->glyph->bitmap.rows;
			chr.bearing_x=face->glyph->bitmap_left;
			chr.bearing_y=face->glyph->bitmap_top;
			chr.advance=face->glyph->advance.x>>6;

			characters.insert( make_pair( t[i],chr ) );

			dirty=true;
		}
	}

	return dirty;
}

void BBImageFont::rebuildAtlas(){
	if( !dirty ) return;

	delete atlas;
	atlas=d_new BBPixmap;
	atlas->width=atlas->height=256;
	atlas->bits=new unsigned char[atlas->width*atlas->height];
	memset( atlas->bits,0,atlas->width*atlas->height );

	int ox=0,oy=0,my=0;
	for( map<char,Char>::iterator it=characters.begin();it!=characters.end();++it ){
		Char &c=it->second;

		FT_Load_Glyph( face,c.index,FT_LOAD_RENDER );

		if( ox+c.width>=atlas->width ){
			oy+=my;
			ox=my=0;
		}

		c.x=ox;c.y=oy;

		int width=face->glyph->bitmap.width;

		my=max( c.height,my );

		for( int y=0;y<face->glyph->bitmap.rows;y++ ){
			memcpy( atlas->bits+(atlas->width*(oy+y))+ox,face->glyph->bitmap.buffer+y*width,width );
		}

		ox+=c.width;
	}

	dirty=false;
}

BBImageFont::Char &BBImageFont::getChar( char c ){
	loadChars( string( 1,c ) );
	return characters[c];
}

int BBImageFont::getKerning( char l,char r ){
	if( !FT_HAS_KERNING(face) ) return 0;
	Char lc=getChar( l ),rc=getChar( r );

	FT_Vector delta;
	FT_Get_Kerning( face,lc.index,rc.index,FT_KERNING_DEFAULT,&delta );
	return delta.x>>6;
}

int BBImageFont::getWidth()const{
	return 0;
}

int BBImageFont::getHeight()const{
	return baseline;
}

int BBImageFont::getWidth( const std::string &text )const{
	loadChars( text );

	int width=0;
	for( int i=0;i<text.length();i++ ){
		width+=characters[text[i]].advance;
	}
	return width;
}

bool BBImageFont::isPrintable( int chr )const{
	return true;
}
