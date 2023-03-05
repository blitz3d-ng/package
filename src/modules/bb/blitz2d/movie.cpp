
#include <bb/runtime/runtime.h>
#include "blitz2d.h"

extern BBGraphics *gx_graphics;
#define b2d_graphics ((B2DGraphics*)gx_graphics)

BBMovie::~BBMovie(){
}

BBMovie * BBCALL bbOpenMovie( BBStr *s ){
	BBMovie *movie=b2d_graphics->openMovie( *s,0 );delete s;
	return movie;
}

bb_int_t BBCALL bbDrawMovie( BBMovie *movie,bb_int_t x,bb_int_t y,bb_int_t w,bb_int_t h ){
	if( w<0 ) w=movie->getWidth();
	if( h<0 ) h=movie->getHeight();
	int playing=movie->draw( gx_canvas,x,y,w,h );
	if( !bbRuntimeIdle() ) RTEX( 0 );
	return playing;
}

bb_int_t BBCALL bbMovieWidth( BBMovie *movie ){
	return movie->getWidth();
}

bb_int_t BBCALL bbMovieHeight( BBMovie *movie ){
	return movie->getHeight();
}

bb_int_t BBCALL bbMoviePlaying( BBMovie *movie ){
	return movie->isPlaying();
}

void BBCALL bbCloseMovie( BBMovie *movie ){
	b2d_graphics->closeMovie( movie );
}
