#ifndef BB_GRAPHICS_D3D7_DRIVER_H
#define BB_GRAPHICS_D3D7_DRIVER_H

#include <bb/graphics/graphics.h>
#include <bb/runtime.windows/gxgraphics.h>
#include <ddraw.h>
#include <vector>

class D3D7ContextDriver : public BBContextDriver{
public:
  D3D7ContextDriver( HWND hwnd );
  ~D3D7ContextDriver();

  struct GfxMode;
  struct GfxDriver;

protected:
  bool enum_all;
  std::vector<GfxDriver*> drivers;
  GfxDriver *curr_driver;

	//for modes 1 and 2
	int mod_cnt;
	MMRESULT timerID;
	IDirectDrawClipper *clipper;
	IDirectDrawSurface7 *primSurf;

	static void CALLBACK timerCallback( UINT id,UINT msg,DWORD_PTR driver,DWORD dw1,DWORD dw2 );
	bool createInvalidationTimer();
	void freeInvalidationTimer();
	void invalidateRect();
	void paint();

  IDirectDraw7 *createDD();

  void enumGfx();
  void denumGfx();

  bool setDisplayMode( int w,int h,int d,bool d3d,IDirectDraw7 *dd );
	gxGraphics *openWindowedGraphics( int w,int h,int d,bool d3d );
	gxGraphics *openExclusiveGraphics( int w,int h,int d,bool d3d );

	bool exclusive;
	HWND hwnd;

public:
  int numGraphicsDrivers();
  void graphicsDriverInfo( int driver,std::string *name,int *c );
  int numGraphicsModes( int driver );
  void graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c );
  void windowedModeInfo( int *c );

	void flip( bool vwait );
};

#endif
