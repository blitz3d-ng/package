#ifndef BBGRAPHICS_H
#define BBGRAPHICS_H

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

  //MANIPULATORS
  virtual void vwait()=0;
  virtual void flip( bool vwait )=0;

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

  // gxMovie *openMovie( const std::string &file,int flags )=0;
  // gxMovie *verifyMovie( gxMovie *movie )=0;
  // void closeMovie( gxMovie *movie )=0;

  // BBFont *loadFont( const std::string &font,int height,int flags )=0;
  // BBFont *verifyFont( BBFont *font )=0;
  // void freeFont( BBFont *font )=0;

  // gxScene *createScene( int flags )=0;
  // gxScene *verifyScene( gxScene *scene )=0;
  // void freeScene( gxScene *scene )=0;
  //
  // gxMesh *createMesh( int max_verts,int max_tris,int flags )=0;
  // gxMesh *verifyMesh( gxMesh *mesh )=0;
  // void freeMesh( gxMesh *mesh )=0;
};

#endif
