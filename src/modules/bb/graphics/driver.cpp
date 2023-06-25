#include "graphics.h"

BBMovie *BBGraphics::verifyMovie( BBMovie *m ){
	return movie_set.count( m ) ? m : 0;
}

void BBGraphics::closeMovie( BBMovie *m ){
	if( movie_set.erase( m ) ) delete m;
}

BBFont *BBGraphics::loadFont( const std::string &font,int height,int flags ){
	BBFont *f=BBImageFont::load( font,height,getDensity(),flags );
	font_set.insert( f );
	return f;
}

BBFont *BBGraphics::verifyFont( BBFont *f ){
	return font_set.count( f ) ? f : 0;
}

void BBGraphics::freeFont( BBFont *f ){
	if( font_set.erase( f ) ) delete f;
}
