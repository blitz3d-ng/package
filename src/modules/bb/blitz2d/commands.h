#ifndef BB_BLITZ2D_COMMANDS_H
#define BB_BLITZ2D_COMMANDS_H

#include <bb/graphics/commands.h>

#ifdef __cplusplus
extern "C" {
#else
typedef void BBFont;
typedef void BBMovie;
#endif

//2d rendering functions
void	 BBCALL bbOrigin( int x,int y );
void	 BBCALL bbViewport( int x,int y,int w,int h );
void	 BBCALL bbColor( int r,int g,int b );
void	 BBCALL bbClsColor( int r,int g,int b );
void	 BBCALL bbCls();
void	 BBCALL bbPlot( int x,int y );
void	 BBCALL bbLine( int x1,int y1,int x2,int y2 );
void	 BBCALL bbRect( int x,int y,int w,int h,int solid );
void	 BBCALL bbOval( int x,int y,int w,int h,int solid );
void	 BBCALL bbText( int x,int y,BBStr *str,int centre_x,int centre_y );
void	 BBCALL bbGetColor( int x,int y );
int		 BBCALL bbColorRed();
int		 BBCALL bbColorGreen();
int		 BBCALL bbColorBlue();

//font functions
BBFont * BBCALL bbLoadFont( BBStr *name,int height,int bold,int italic,int underline );
void	 BBCALL bbFreeFont( BBFont *f );
void	 BBCALL bbSetFont( BBFont *f );
int		 BBCALL bbFontWidth();
int		 BBCALL bbFontHeight();
int		 BBCALL bbStringWidth( BBStr *str );
int		 BBCALL bbStringHeight( BBStr *str );

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
