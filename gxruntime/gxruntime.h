
#ifndef GXRUNTIME_H
#define GXRUNTIME_H

#include <bb/frame/frame.h>
#include <string>
#include <vector>

#include "gxgraphics.h"

#include "../debugger/debugger.h"

class gxRuntime : public Frame{
	/***** INTERNAL INTERFACE *****/
public:
	BBEnv env;

	HINSTANCE hinst;

	gxGraphics *graphics;

	void flip( bool vwait );
	void moveMouse( int x,int y );

	LRESULT windowProc( HWND hwnd,UINT msg,WPARAM w,LPARAM l );

	struct GfxMode;
	struct GfxDriver;

private:
	gxRuntime( HINSTANCE hinst,HWND hwnd );
	~gxRuntime();

	void refreshSystemProperties();

	void paint();
	void suspend();
	void forceSuspend();
	void resume();
	void forceResume();

	bool pointer_visible;

	bool setDisplayMode( int w,int h,int d,bool d3d,IDirectDraw7 *dd );
	gxGraphics *openWindowedGraphics( int w,int h,int d,bool d3d );
	gxGraphics *openExclusiveGraphics( int w,int h,int d,bool d3d );

	bool enum_all;
	std::vector<GfxDriver*> drivers;
	GfxDriver *curr_driver;
	int use_di;

	void enumGfx();
	void denumGfx();

	void backupGraphics();
	void restoreGraphics();
	void acquireInput();
	void unacquireInput();

	/***** APP INTERFACE *****/
public:
	static gxRuntime *openRuntime( HINSTANCE hinst,Debugger *debugger );
	static void closeRuntime( gxRuntime *runtime );

	void asyncStop();
	void asyncRun();
	void asyncEnd();

	void invalidateRect();

	/***** GX INTERFACE *****/
public:
	enum{
		GFXMODECAPS_3D=1
	};

	//return true if program should continue, or false for quit.
	bool idle();

	void setPointerVisible( bool vis );

	void debugStop();
	void debugProfile( int per );
	void debugStmt( int pos,const char *file );
	void debugEnter( void *frame,void *env,const char *func );
	void debugLeave();
	void debugInfo( const char *t );
	void debugError( const char *t );
	void debugLog( const char *t );

	int numGraphicsDrivers();
	void graphicsDriverInfo( int driver,std::string *name,int *caps );

	int numGraphicsModes( int driver );
	void graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *caps );

	void dpiInfo( float *scale_x,float *scale_y );

	void windowedModeInfo( int *caps );

	BBGraphics *openGraphics( int w,int h,int d,int driver,int flags );
	void closeGraphics( BBGraphics *graphics );
	bool graphicsLost();

	void enableDirectInput( bool use );
	int  directInputEnabled(){ return use_di; }

	OSVERSIONINFO osinfo;
};

#endif
