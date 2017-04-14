#ifndef BB_GRAPHICS_D3D7_H
#define BB_GRAPHICS_D3D7_H

#include <bb/graphics/graphics.h>
#include <ddraw.h>
#include <vector>

class D3D7ContextDriver : public BBContextDriver{
public:
  D3D7ContextDriver();
  ~D3D7ContextDriver();

  struct GfxMode;
  struct GfxDriver;

protected:
  bool enum_all;
  std::vector<GfxDriver*> drivers;
  GfxDriver *curr_driver;

  IDirectDraw7 *createDD();

  void enumGfx();
  void denumGfx();

  bool setDisplayMode( int w,int h,int d,bool d3d,IDirectDraw7 *dd );
public:

  int numGraphicsDrivers();
  void graphicsDriverInfo( int driver,std::string *name,int *c );
  int numGraphicsModes( int driver );
  void graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c );
  void windowedModeInfo( int *c );
};

#endif
