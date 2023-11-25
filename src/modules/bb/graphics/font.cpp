
#include "../stdutil/stdutil.h"
#include <bb/system/system.h>
#include "font.h"

#undef max

FT_Library ft;
std::map<std::string,BBFontData> bbFontCache;

BBFont::~BBFont(){
}

BBImageFont::BBImageFont( FT_Face f,int h,float d ):face(f),height(h),atlas(0){
	FT_Size_RequestRec req={ FT_SIZE_REQUEST_TYPE_REAL_DIM,0,(long)(height*64*d),0,0 };
	FT_Request_Size( face,&req );

	baseline=height*d+(face->size->metrics.descender/64);
	density=1.0/d;
}

BBImageFont *BBImageFont::load( const std::string &name,int height,float density,int flags ){
	BBFontData font;
	if( bbFontCache.count( name )==0 ){
		int n=name.rfind( "." );
		if( n==std::string::npos ){
			if( !bbSystemDriver->lookupFontData( name,font ) ){
				return 0;
			}
		}else{
			// TODO: needs more work
			std::string ext=tolower( name.substr( n+1 ) );
			if( ext=="ttf" ){
				FILE *in=fopen( name.c_str(),"rb" );
				if( !in ) return 0;

				fseek( in,0,SEEK_END );
				font.size=ftell( in );
				fseek( in,0,SEEK_SET );

				font.data=(unsigned char *)malloc( font.size );
				fread( font.data,font.size,1,in );
				fclose( in );
			}
		}
	}else{
		font=bbFontCache[name];
	}

	bbFontCache.insert( make_pair( name,font ) );

	FT_Face face;
	if( FT_New_Memory_Face( ft,font.data,font.size,0,&face ) ){
		return 0;
	}
	return d_new BBImageFont( face,height,density );
}

bool BBImageFont::loadChars( const std::string &t )const{
	const char *c=t.c_str();
	while( *c ){
		utf8_int32_t code;
		auto b=c;
		c=utf8codepoint( c,&code );

		if( !characters.count(code) ){
			LOGD("c: %c,%i => %i", *b,*b,code);

			Char chr;
			chr.index=FT_Get_Char_Index( face,code );
			FT_Load_Glyph( face,chr.index,FT_LOAD_RENDER );

			chr.width=face->glyph->bitmap.width;
			chr.height=face->glyph->bitmap.rows;
			chr.bearing_x=face->glyph->bitmap_left;
			chr.bearing_y=face->glyph->bitmap_top;
			chr.advance=face->glyph->advance.x>>6;

			characters.insert( std::make_pair( code,chr ) );

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
	for( std::map<utf8_int32_t,Char>::iterator it=characters.begin();it!=characters.end();++it ){
		Char &c=it->second;

		FT_Load_Glyph( face,c.index,FT_LOAD_RENDER );

		if( ox+c.width>=atlas->width ){
			oy+=my;
			ox=my=0;
		}

		c.x=ox;c.y=oy;

		int width=face->glyph->bitmap.width;

		my=std::max( c.height,my );

		for( int y=0;y<face->glyph->bitmap.rows;y++ ){
			memcpy( atlas->bits+(atlas->width*(oy+y))+ox,face->glyph->bitmap.buffer+y*width,width );
		}

		ox+=c.width;
	}

	dirty=false;
}

BBImageFont::Char &BBImageFont::getChar( utf8_int32_t c ){
	loadChars( std::string( 1,c ) );
	return characters[c];
}

float BBImageFont::getKerning( utf8_int32_t l,utf8_int32_t r ){
	if( !FT_HAS_KERNING(face) ) return 0;
	Char lc=getChar( l ),rc=getChar( r );

	FT_Vector delta;
	FT_Get_Kerning( face,lc.index,rc.index,FT_KERNING_DEFAULT,&delta );
	return (delta.x>>6)*density;
}

int BBImageFont::getWidth()const{
	return 0;
}

int BBImageFont::getHeight()const{
	return height;
}

int BBImageFont::getWidth( const std::string &text )const{
	loadChars( text );

	int width=0;
	for( int i=0;i<text.length();i++ ){
		width+=characters[text[i]].advance*density;
	}
	return width;
}

bool BBImageFont::isPrintable( int chr )const{
	return FT_Get_Char_Index( face,chr )!=0;
}
