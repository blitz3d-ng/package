#ifndef BBGRAPHICS_H
#define BBGRAPHICS_H

#include <bb/blitz/blitz.h>
#include "canvas.h"

class BBGraphics{
  /***** GX INTERFACE *****/
public:
  enum{
    GRAPHICS_WINDOWED=1,	//windowed mode
    GRAPHICS_SCALED=2,		//scaled window
    GRAPHICS_3D=4,			//3d mode! Hurrah!
    GRAPHICS_AUTOSUSPEND=8	//suspend graphics when app suspended
  };

	virtual void backup()=0;
	virtual bool restore()=0;

  //MANIPULATORS
  virtual void vwait()=0;

  //SPECIAL!
  virtual void copy( BBCanvas *dest,int dx,int dy,int dw,int dh,BBCanvas *src,int sx,int sy,int sw,int sh )=0;

  //NEW! Gamma control!
  virtual void setGamma( int r,int g,int b,float dr,float dg,float db )=0;
  virtual void getGamma( int r,int g,int b,float *dr,float *dg,float *db )=0;
  virtual void updateGamma( bool calibrate )=0;

  //ACCESSORS
  virtual int getWidth()const=0;
  virtual int getHeight()const=0;
  virtual int getDepth()const=0;
  virtual int getScanLine()const=0;
  virtual int getAvailVidmem()const=0;
  virtual int getTotalVidmem()const=0;

  virtual BBCanvas *getFrontCanvas()const=0;
  virtual BBCanvas *getBackCanvas()const=0;
  virtual BBFont *getDefaultFont()const=0;

  //OBJECTS
  virtual BBCanvas *createCanvas( int width,int height,int flags )=0;
  virtual BBCanvas *loadCanvas( const std::string &file,int flags )=0;
  virtual BBCanvas *verifyCanvas( BBCanvas *canvas )=0;
  virtual void freeCanvas( BBCanvas *canvas )=0;
};

class BBContextDriver{
public:
	BBContextDriver();
protected:
	BBGraphics *graphics;

public:
  enum{
    GFXMODECAPS_3D=1
  };

	bool graphicsOpened();

  virtual int numGraphicsDrivers()=0;
  virtual void graphicsDriverInfo( int driver,std::string *name,int *c )=0;
  virtual int numGraphicsModes( int driver )=0;
  virtual void graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c )=0;
  virtual void windowedModeInfo( int *c )=0;

  virtual BBGraphics *openGraphics( int w,int h,int d,int driver,int flags )=0;
  virtual void closeGraphics( BBGraphics *graphics )=0;
  virtual bool graphicsLost()=0;

  virtual void flip( bool vwait )=0;
};

class bbImage;

//general graphics functions
int		 BBCALL bbCountGfxDrivers();
BBStr *	 BBCALL bbGfxDriverName( int n );
BBStr *	 BBCALL bbGfxDriverDesc( int n );
void	 BBCALL bbSetGfxDriver( int n );
int		 BBCALL bbGfxModeExists( int w,int h,int d );
int		 BBCALL bbCountGfxModes();
int		 BBCALL bbGfxModeWidth( int n );
int		 BBCALL bbGfxModeHeight( int n );
int		 BBCALL bbGfxModeDepth( int n );
int		 BBCALL bbGraphicsWidth();
int		 BBCALL bbGraphicsHeight();
int		 BBCALL bbGraphicsDepth();
int		 BBCALL bbAvailVidMem();
int		 BBCALL bbTotalVidMem();

//mode functions
void	 BBCALL bbGraphics( int w,int h,int d,int mode );
BBCanvas * BBCALL bbFrontBuffer();
BBCanvas * BBCALL bbBackBuffer();
void	 BBCALL bbEndGraphics();
int		 BBCALL bbGraphicsLost();
int		 BBCALL bbGraphicsOpen();
int		 BBCALL bbScanLine();
void	 BBCALL bbVWait( int n );
void	 BBCALL bbFlip( int vwait );

// misc
void BBCALL bbMoveMouse( int x,int y );

//graphics buffer functions
void	 BBCALL bbSetBuffer( BBCanvas *buff );
BBCanvas * BBCALL bbGraphicsBuffer();
int		 BBCALL bbLoadBuffer( BBCanvas *surf,BBStr *str );
int		 BBCALL bbSaveBuffer( BBCanvas *surf,BBStr *str );

//fast read/write operations...
void	 BBCALL bbLockBuffer( BBCanvas *buff );
void	 BBCALL bbUnlockBuffer( BBCanvas *buff );
int		 BBCALL bbReadPixel( int x,int y,BBCanvas *buff );
void	 BBCALL bbWritePixel( int x,int y,int argb,BBCanvas *buff );
int		 BBCALL bbReadPixelFast( int x,int y,BBCanvas *buff );
void	 BBCALL bbWritePixelFast( int x,int y,int argb,BBCanvas *buff );


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

//image functions
bbImage* BBCALL bbLoadImage( BBStr *s );
bbImage* BBCALL bbCopyImage( bbImage *i );
bbImage* BBCALL bbCreateImage( int w,int h,int n );
bbImage* BBCALL bbLoadAnimImage( BBStr *s,int w,int h,int first,int cnt );
void	 BBCALL bbFreeImage( bbImage *i );
int		 BBCALL bbSaveImage( bbImage *i,BBStr *filename,int frame );
void	 BBCALL bbGrabImage( bbImage *i,int x,int y,int n );
BBCanvas * BBCALL bbImageBuffer( bbImage *i,int n );
void	 BBCALL bbDrawImage( bbImage *i,int x,int y,int frame );
void	 BBCALL bbDrawBlock( bbImage *i,int x,int y,int frame );
void	 BBCALL bbTileImage( bbImage *i,int x,int y,int frame );
void	 BBCALL bbTileBlock( bbImage *i,int x,int y,int frame );
void	 BBCALL bbDrawImageRect( bbImage *i,int x,int y,int r_x,int r_y,int r_w,int r_h,int frame );
void	 BBCALL bbDrawBlockRect( bbImage *i,int x,int y,int r_x,int r_y,int r_w,int r_h,int frame );
void	 BBCALL bbMaskImage( bbImage *i,int r,int g,int b );
void	 BBCALL bbHandleImage( bbImage *i,int x,int y );
void	 BBCALL bbScaleImage( bbImage *i,float w,float h );
void	 BBCALL bbResizeImage( bbImage *i,float w,float h );
void	 BBCALL bbRotateImage( bbImage *i,float angle );
void	 BBCALL bbTFormImage( bbImage *i,float a,float b,float c,float d );
void	 BBCALL bbTFormFilter( int enable );
void	 BBCALL bbAutoMidHandle( int enable );
void	 BBCALL bbMidHandle( bbImage *i );
int		 BBCALL bbImageWidth( bbImage *i );
int		 BBCALL bbImageHeight( bbImage *i );
int		 BBCALL bbImageXHandle( bbImage *i );
int		 BBCALL bbImageYHandle( bbImage *i );
int		 BBCALL bbImagesOverlap( bbImage *i1,int x1,int y1,bbImage *i2,int x2,int y2 );
int		 BBCALL bbImagesCollide( bbImage *i1,int x1,int y1,int f1,bbImage *i2,int x2,int y2,int f2 );
int		 BBCALL bbRectsOverlap( int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2 );
int		 BBCALL bbImageRectOverlap( bbImage *i,int x,int y,int r_x,int r_y,int r_w,int r_h );
int		 BBCALL bbImageRectCollide( bbImage *i,int x,int y,int f,int r_x,int r_y,int r_w,int r_h );

//simple print functions
void	 BBCALL bbWrite( BBStr *str );
void	 BBCALL bbPrint( BBStr *str );
BBStr *	 BBCALL bbInput( BBStr *prompt );
void	 BBCALL bbLocate( int x,int y );

extern BBGraphics *gx_graphics;
extern BBCanvas *gx_canvas;

extern BBContextDriver *bbContextDriver;

#endif
