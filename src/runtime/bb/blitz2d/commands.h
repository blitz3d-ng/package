#ifndef BB_BLITZ2D_COMMANDS_H
#define BB_BLITZ2D_COMMANDS_H

#include <bb/blitz/commands.h>

#ifdef __cplusplus
extern "C" {
#else
typedef void BBMovie;
#endif

BBMovie * BBCALL bbOpenMovie( BBStr *s );
int BBCALL bbDrawMovie( BBMovie *movie,int x,int y,int w,int h );
int BBCALL bbMovieWidth( BBMovie *movie );
int BBCALL bbMovieHeight( BBMovie *movie );
int BBCALL bbMoviePlaying( BBMovie *movie );
void BBCALL bbCloseMovie( BBMovie *movie );

#ifdef __cplusplus
}
#endif

#endif
