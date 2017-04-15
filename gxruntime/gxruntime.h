
#ifndef GXRUNTIME_H
#define GXRUNTIME_H

#include <bb/frame/frame.h>
#include <bb/graphics.d3d7/graphics.d3d7.h>
#include <string>
#include <vector>

#include "../debugger/debugger.h"

class gxRuntime : public D3D7ContextDriver, public Frame{
	/***** INTERNAL INTERFACE *****/
public:
	BBEnv env;

	HINSTANCE hinst;

	void moveMouse( int x,int y );

	LRESULT windowProc( HWND hwnd,UINT msg,WPARAM w,LPARAM l );

private:
	gxRuntime( HINSTANCE hinst,HWND hwnd );
	~gxRuntime();

	void refreshSystemProperties();

	void suspend();
	void forceSuspend();
	void resume();
	void forceResume();

	bool pointer_visible;

	int use_di;

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

	/***** GX INTERFACE *****/
public:
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

	BBGraphics *openGraphics( int w,int h,int d,int driver,int flags );
	void closeGraphics( BBGraphics *graphics );
	bool graphicsLost();

	void enableDirectInput( bool use );
	int  directInputEnabled(){ return use_di; }
};

#endif
