
#include <bb/runtime/runtime.h>
#include "graphics.h"
#include "movie.h"

extern BBGraphics *gx_graphics;
#define b2d_graphics ((B2DGraphics*)gx_graphics)

BBMovie::~BBMovie(){
}

BBMovie * BBCALL bbOpenMovie( BBStr *s ){
	BBMovie *movie=b2d_graphics->openMovie( *s,0 );delete s;
	return movie;
}

int BBCALL bbDrawMovie( BBMovie *movie,int x,int y,int w,int h ){
	if( w<0 ) w=movie->getWidth();
	if( h<0 ) h=movie->getHeight();
	int playing=movie->draw( gx_canvas,x,y,w,h );
	if( !bbRuntimeIdle() ) RTEX( 0 );
	return playing;
}

int BBCALL bbMovieWidth( BBMovie *movie ){
	return movie->getWidth();
}

int BBCALL bbMovieHeight( BBMovie *movie ){
	return movie->getHeight();
}

int BBCALL bbMoviePlaying( BBMovie *movie ){
	return movie->isPlaying();
}

void BBCALL bbCloseMovie( BBMovie *movie ){
	b2d_graphics->closeMovie( movie );
}
