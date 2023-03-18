#include "graphics.h"

B2DGraphics::~B2DGraphics(){
  while( movie_set.size() ) closeMovie( *movie_set.begin() );
  while( font_set.size() ) freeFont( *font_set.begin() );
}

BBMovie *B2DGraphics::verifyMovie( BBMovie *m ){
	return movie_set.count( m ) ? m : 0;
}

void B2DGraphics::closeMovie( BBMovie *m ){
	if( movie_set.erase( m ) ) delete m;
}

BBFont *B2DGraphics::verifyFont( BBFont *f ){
	return font_set.count( f ) ? f : 0;
}

void B2DGraphics::freeFont( BBFont *f ){
	if( font_set.erase( f ) ) delete f;
}
