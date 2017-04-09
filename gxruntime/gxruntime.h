
#ifndef GXRUNTIME_H
#define GXRUNTIME_H

#include <windows.h>
#include <string>
#include <vector>

#include <bb/audio.fmod/driver.h>
#include <bb/input.directinput8/driver.h>
#include "gxgraphics.h"
#include <bb/timer/timer.h>

#include "../debugger/debugger.h"

class gxRuntime{
	/***** INTERNAL INTERFACE *****/
public:
	BBEnv env;

	HWND hwnd;
	HINSTANCE hinst;

	FMODAudioDriver *audio;
	DirectInput8Driver *input;
	gxGraphics *graphics;

	void flip( bool vwait );
	void moveMouse( int x,int y );

	LRESULT windowProc( HWND hwnd,UINT msg,WPARAM w,LPARAM l );

	struct GfxMode;
	struct GfxDriver;

private:
	gxRuntime( HINSTANCE hinst,const std::string &cmd_line,HWND hwnd );
	~gxRuntime();

	void paint();
	void suspend();
	void forceSuspend();
	void resume();
	void forceResume();
	void backupWindowState();
	void restoreWindowState();

	RECT t_rect;
	int t_style;
	std::string cmd_line;
	bool pointer_visible;
	std::string app_title;
	std::string app_close;

	bool setDisplayMode( int w,int h,int d,bool d3d,IDirectDraw7 *dd );
	gxGraphics *openWindowedGraphics( int w,int h,int d,bool d3d );
	gxGraphics *openExclusiveGraphics( int w,int h,int d,bool d3d );

	bool enum_all;
	std::vector<GfxDriver*> drivers;
	GfxDriver *curr_driver;
	int use_di;

	void enumGfx();
	void denumGfx();

	void resetInput();
	void pauseAudio();
	void resumeAudio();
	void backupGraphics();
	void restoreGraphics();
	void acquireInput();
	void unacquireInput();

	/***** APP INTERFACE *****/
public:
	static gxRuntime *openRuntime( HINSTANCE hinst,const std::string &cmd_line,Debugger *debugger );
	static void closeRuntime( gxRuntime *runtime );

	void asyncStop();
	void asyncRun();
	void asyncEnd();

	/***** GX INTERFACE *****/
public:
	enum{
		GFXMODECAPS_3D=1
	};

	//return true if program should continue, or false for quit.
	bool idle();
	bool delay( int ms );

	bool execute( const std::string &cmd );
	void setTitle( const std::string &title,const std::string &close );
	int  getMilliSecs();
	void setPointerVisible( bool vis );

	std::string commandLine();

	std::string systemProperty( const std::string &t );

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

	BBAudioDriver *openAudio( int flags );
	void closeAudio( BBAudioDriver *audio );

	BBInputDriver *openInput( int flags );
	void closeInput( BBInputDriver *input );

	BBGraphics *openGraphics( int w,int h,int d,int driver,int flags );
	void closeGraphics( BBGraphics *graphics );
	bool graphicsLost();

	BBTimer *createTimer( int hertz );
	void freeTimer( BBTimer *timer );

	void enableDirectInput( bool use );
	int  directInputEnabled(){ return use_di; }

	OSVERSIONINFO osinfo;
};

#endif
