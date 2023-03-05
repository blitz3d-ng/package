#ifndef BBGRAPHICS_H
#define BBGRAPHICS_H

#include <bb/blitz/blitz.h>
#include "canvas.h"
#include <set>

class BBGraphics{
protected:
  std::set<BBCanvas*> canvas_set;

  BBCanvas *front_canvas,*back_canvas;

  /***** GX INTERFACE *****/
public:
  BBGraphics();
  virtual ~BBGraphics();

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
  virtual int getLogicalWidth()const=0;
  virtual int getLogicalHeight()const=0;
  virtual int getDepth()const=0;
  virtual float getDensity()const=0;
  virtual int getScanLine()const=0;
  virtual int getAvailVidmem()const=0;
  virtual int getTotalVidmem()const=0;

  BBCanvas *getFrontCanvas()const;
  BBCanvas *getBackCanvas()const;

  virtual BBFont *getDefaultFont()const=0;

  //OBJECTS
  virtual BBCanvas *createCanvas( int width,int height,int flags )=0;
  virtual BBCanvas *loadCanvas( const std::string &file,int flags )=0;
  BBCanvas *verifyCanvas( BBCanvas *canvas );
  void freeCanvas( BBCanvas *canvas );
};

class BBContextDriver{
public:
	BBContextDriver();
	virtual ~BBContextDriver();
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

#include "commands.h"

extern BBGraphics *gx_graphics;
extern BBCanvas *gx_canvas;

extern BBContextDriver *bbContextDriver;

#endif
