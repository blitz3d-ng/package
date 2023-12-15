
#include "../stdutil/stdutil.h"
#include "font.h"
#include <utf8.h>

#undef max

struct BBFontData{
	long size;
	unsigned char *data;

	BBFontData():size(0),data(0){}
};

#ifdef BB_WINDOWS
bool lookupFontData( const std::string &fontName,BBFontData &font ){
	bool bold=false,italic=false,underline=false,strikeout=false;

	int height=10;

	HFONT hfont=CreateFont(
		height,0,0,0,
		bold,italic,underline,strikeout,
		ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,fontName.c_str() );

	if( !hfont ) return false;

	bool success=false;
	HDC hdc=CreateCompatibleDC( NULL );
	if( hdc ){
		if( SelectObject( hdc,hfont ) ) {
			font.size=GetFontData( hdc,0,0,NULL,0 );
			if( font.size>0 ) {
				font.data=new unsigned char[font.size];
				if( GetFontData( hdc,0,0,(void*)font.data,font.size )==font.size ){
					success=true;
				}else{
					delete[] font.data;
				}
			}
		}
		DeleteDC( hdc );
	}

	DeleteObject( hfont );

	return success;
}
#else
#ifdef BB_MACOS
extern "C" const char *lookupFontFile( const char *fontName );

bool lookupFontData( const std::string &fontName,BBFontData &font ){
	const char *fontPath=lookupFontFile( fontName.c_str() );
	if( !fontPath ) return false;

	std::ifstream file( fontPath,std::ios::binary|std::ios::ate );
	font.size=file.tellg();
	file.seekg( 0,std::ios::beg );

	font.data=new unsigned char[font.size];
	if( file.read( (char*)font.data,font.size ) ){
		return true;
	}
	return false;
}
#else
#ifdef BB_LINUX
#include <fontconfig/fontconfig.h>

bool lookupFontData( const std::string &fontName,BBFontData &font ){
	static FcConfig* config=0;
	if( !config ) config=FcInitLoadConfigAndFonts();

	FcPattern* pat=FcNameParse( (const FcChar8*)fontName.c_str() );
	FcConfigSubstitute( config,pat,FcMatchPattern );
	FcDefaultSubstitute( pat );
	char* fontPath=0;
	FcResult result;
	FcPattern* match=FcFontMatch( config,pat,&result );
	if( match ){
		FcChar8* file = NULL;
		if( FcPatternGetString( match,FC_FILE,0,&file )==FcResultMatch ){
			fontPath = (char*)file;
		}
	}
	FcPatternDestroy( pat );

	if( !fontPath ) return false;

	std::ifstream file( fontPath,std::ios::binary|std::ios::ate );
	font.size=file.tellg();
	file.seekg( 0,std::ios::beg );

	font.data=new unsigned char[font.size];
	if( file.read( (char*)font.data,font.size ) ){
		return true;
	}
	return false;
}
#else
bool lookupFontData( const std::string &fontName,BBFontData &font ){
	return false;
}
#endif
#endif
#endif

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
			if( !lookupFontData( name,font ) ){
				return 0;
			}
		}else{
			// TODO: needs more work
			std::string ext=tolower( name.substr( n+1 ) );
			if( ext=="ttf"||ext=="fon" ){
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

	if( !face->charmap ){
		FT_Set_Charmap( face,face->charmaps[0] );
	}

	return d_new BBImageFont( face,height,density );
}

bool BBImageFont::loadChar( uint32_t c )const{
	if( characters.count( c ) ) return false;

	Char chr;
	chr.index=FT_Get_Char_Index( face,c );
	FT_Load_Glyph( face,chr.index,FT_LOAD_RENDER );

	chr.width=face->glyph->bitmap.width;
	chr.height=face->glyph->bitmap.rows;
	chr.bearing_x=face->glyph->bitmap_left;
	chr.bearing_y=face->glyph->bitmap_top;
	chr.advance=face->glyph->advance.x>>6;

	characters.insert( std::make_pair( c,chr ) );

	return (dirty=true);
}

bool BBImageFont::loadChars( const std::string &t )const{
	const char *s=t.c_str();
	while( *s ){
		utf8_int32_t chr;
		s=utf8codepoint( s,&chr );
		loadChar( chr );
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
	for( std::map<uint32_t,Char>::iterator it=characters.begin();it!=characters.end();++it ){
		Char &c=it->second;

		FT_Load_Glyph( face,c.index,FT_LOAD_RENDER );

		if( ox+c.width>=atlas->width ){
			oy+=my;
			ox=my=0;
		}

		c.x=ox;c.y=oy;

		my=std::max( c.height,my );

		FT_Bitmap bm=face->glyph->bitmap;
		for( int y=0;y<bm.rows;y++ ){
			unsigned char *in=bm.buffer+y*bm.pitch;
			unsigned char *out=atlas->bits+(atlas->width*(oy+y))+ox;
			switch( bm.pixel_mode ){
			case FT_PIXEL_MODE_MONO:
				for( int i=0;i<8;i++ ) out[i]=(*in>>(7-i))&1?0xff:0;
				break;
			case FT_PIXEL_MODE_GRAY:
				memcpy( out,in,bm.width );
				break;
			default:
				RTEX( "unhandled font bitmap format" );
			}
		}

		ox+=c.width;
	}

	dirty=false;
}

BBImageFont::Char &BBImageFont::getChar( uint32_t c ){
	loadChar( c );
	return characters[c];
}

float BBImageFont::getKerning( uint32_t l,uint32_t r ){
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

	const char *t=text.c_str();
	int w=0;
	while( *t ){
		utf8_int32_t c;
		t=utf8codepoint( t,&c );
		w+=characters[c].advance*density;
	}
	return w;
}

bool BBImageFont::isPrintable( int chr )const{
	return FT_Get_Char_Index( face,chr )!=0;
}
