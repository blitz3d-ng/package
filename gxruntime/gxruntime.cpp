
#include "std.h"
#include "gxruntime.h"
#include <bb/blitz/app.h>
#include <bb/runtime/runtime.h>
#include <bb/input.directinput8/driver.h>
#include <bb/system/system.h>
#include "gxgraphics.h"
#include "zmouse.h"

#define dx_input ((DirectInput8Driver*)gx_input)

gxRuntime *gx_runtime;

#undef SPI_SETMOUSESPEED
#define SPI_SETMOUSESPEED	113

static gxRuntime *runtime;
static bool busy,suspended;
static volatile bool run_flag;

// TODO: Move these to the proper place...
void *bbRuntimeWindow(){
	return runtime->Frame::hwnd;
}

bool bbRuntimeIdle(){
	return runtime->idle();
}

static LRESULT CALLBACK windowProc( HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam );

static bool gfx_lost;
static bool auto_suspend;

static Debugger *debugger;

enum{
	WM_STOP=WM_USER+1,WM_RUN,WM_END
};

////////////////////
// STATIC STARTUP //
////////////////////
gxRuntime *gxRuntime::openRuntime( HINSTANCE hinst,Debugger *d ){
	if( runtime ) return 0;

	//create debugger
	bbAttachDebugger( (debugger=d) );

	//create WNDCLASS
	WNDCLASS wndclass;
	memset(&wndclass,0,sizeof(wndclass));
	wndclass.style=CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	wndclass.lpfnWndProc=::windowProc;
	wndclass.hInstance=hinst;
	wndclass.lpszClassName="Blitz Runtime Class";
	wndclass.hCursor=(HCURSOR)LoadCursor( 0,IDC_ARROW );
	wndclass.hbrBackground=(HBRUSH)GetStockObject( BLACK_BRUSH );
	RegisterClass( &wndclass );

	busy=suspended=false;
	run_flag=true;

	const char *app_t=" ";
	int ws=WS_CAPTION,ws_ex=0;

	HWND hwnd=CreateWindowEx( ws_ex,"Blitz Runtime Class",app_t,ws,0,0,0,0,0,0,0,0 );

	UpdateWindow( hwnd );

	runtime=d_new gxRuntime( hinst,hwnd );
	bbContextDriver=runtime;
	return runtime;
}

void gxRuntime::closeRuntime( gxRuntime *r ){
	if( !runtime || runtime!=r ) return;

	delete runtime;
	runtime=0;

}

//////////////////////////
// RUNTIME CONSTRUCTION //
//////////////////////////
typedef int (_stdcall *SetAppCompatDataFunc)( int x,int y );

gxRuntime::gxRuntime( HINSTANCE hi,HWND hw ):
hinst(hi),
pointer_visible(true),use_di(false),D3D7ContextDriver(hw),Frame(hw){

	CoInitialize( 0 );

	env.window=Frame::hwnd;

	enumGfx();

	HMODULE ddraw=LoadLibraryA( "ddraw.dll" );
	if( ddraw ){
		SetAppCompatDataFunc SetAppCompatData=(SetAppCompatDataFunc)GetProcAddress( ddraw,"SetAppCompatData" );
		if( SetAppCompatData ) SetAppCompatData( 12,0 );
		FreeLibrary( ddraw );
	}
}

gxRuntime::~gxRuntime(){
	if( graphics ) closeGraphics( graphics );
	denumGfx();
	DestroyWindow( Frame::hwnd );
	UnregisterClass( "Blitz Runtime Class",hinst );

	CoUninitialize();
}

void gxRuntime::backupGraphics(){
	if( auto_suspend ){
		graphics->backup();
	}
}

void gxRuntime::restoreGraphics(){
	if( auto_suspend ){
		if( !graphics->restore() ) gfx_lost=true;
	}
}

void gxRuntime::acquireInput(){
	if( !gx_input ) return;
	if( gfx_mode==3 ){
		if( use_di ){
			use_di=dx_input->acquire();
		}else{
		}
	}
	dx_input->reset();
}

void gxRuntime::unacquireInput(){
	if( !gx_input ) return;
	if( gfx_mode==3 && use_di ) dx_input->unacquire();
	dx_input->reset();
}

/////////////
// SUSPEND //
/////////////
void gxRuntime::suspend(){
	busy=true;
	bbRuntimeOnSuspend->run( this );
	backupGraphics();
	unacquireInput();
	suspended=true;
	busy=false;

	if( gfx_mode==3 ) ShowCursor(1);

	_bbDebugStop();
}

////////////
// RESUME //
////////////
void gxRuntime::resume(){
	if( gfx_mode==3 ) ShowCursor(0);
	busy=true;
	bbRuntimeOnResume->run( this );
	acquireInput();
	restoreGraphics();
	suspended=false;
	busy=false;

	_bbDebugRun();
}

///////////////////
// FORCE SUSPEND //
///////////////////
void gxRuntime::forceSuspend(){
	if( gfx_mode==3 ){
		SetForegroundWindow( GetDesktopWindow() );
		ShowWindow( GetDesktopWindow(),SW_SHOW );
	}else{
		suspend();
	}
}

//////////////////
// FORCE RESUME //
//////////////////
void gxRuntime::forceResume(){
	if( gfx_mode==3 ){
		SetForegroundWindow( Frame::hwnd );
		ShowWindow( Frame::hwnd,SW_SHOWMAXIMIZED );
	}else{
		resume();
	}
}

////////////////
// MOVE MOUSE //
////////////////
void gxRuntime::moveMouse( int x,int y ){
	POINT p;
	RECT rect;
	switch( gfx_mode ){
	case 1:
		GetClientRect( D3D7ContextDriver::hwnd,&rect );
		x=x*(rect.right-rect.left)/graphics->getWidth();
		y=y*(rect.bottom-rect.top)/graphics->getHeight();
	case 2:
		p.x=x;p.y=y;ClientToScreen( D3D7ContextDriver::hwnd,&p );x=p.x;y=p.y;
		break;
	case 3:
		if( use_di ) return;
		break;
	default:
		return;
	}
	SetCursorPos( x,y );
}

/////////////////
// WINDOW PROC //
/////////////////
LRESULT gxRuntime::windowProc( HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam ){

	if( busy ){
		return DefWindowProc( hwnd,msg,wparam,lparam );
	}

	PAINTSTRUCT ps;

	//handle 'special' messages!
	switch( msg ){
	case WM_PAINT:
		if( gfx_mode && !auto_suspend ){
			if( !graphics->restore() ) gfx_lost=true;
		}
		BeginPaint( hwnd,&ps );
		paint();
		EndPaint( hwnd,&ps );
		return DefWindowProc( hwnd,msg,wparam,lparam );
	case WM_ERASEBKGND:
		return gfx_mode ? 1 : DefWindowProc( hwnd,msg,wparam,lparam );
	case WM_CLOSE:
		if( bbApp().close.size() ){
			int n=MessageBox( hwnd,bbApp().close.c_str(),bbApp().title.c_str(),MB_OKCANCEL|MB_ICONWARNING|MB_SETFOREGROUND|MB_TOPMOST );
			if( n!=IDOK ) return 0;
		}
		asyncEnd();
		return 0;
	case WM_SETCURSOR:
		if( !suspended ){
			if( gfx_mode==3 ){
				SetCursor( 0 );
				return 1;
			}else if( !pointer_visible ){
				POINT p;
				GetCursorPos( &p );
				ScreenToClient( hwnd,&p );
				RECT r;GetClientRect( hwnd,&r );
				if( p.x>=0 && p.y>=0 && p.x<r.right && p.y<r.bottom ){
					SetCursor( 0 );
					return 1;
				}
			}
		}
		break;
	case WM_ACTIVATEAPP:
		if( auto_suspend ){
			if( wparam ){
				if( suspended ) resume();
			}else{
				if( !suspended ) suspend();
			}
		}
		break;
	}

	if( !gx_input || suspended ) return DefWindowProc( hwnd,msg,wparam,lparam );

	if( gfx_mode==3 && use_di ){
		use_di=dx_input->acquire();
		return DefWindowProc( hwnd,msg,wparam,lparam );
	}

	static const int MK_ALLBUTTONS=MK_LBUTTON|MK_RBUTTON|MK_MBUTTON;

	//handle input messages
	switch( msg ){
	case WM_LBUTTONDOWN:
		dx_input->wm_mousedown(1);
		SetCapture(hwnd);
		break;
	case WM_LBUTTONUP:
		dx_input->wm_mouseup(1);
		if( !(wparam&MK_ALLBUTTONS) ) ReleaseCapture();
		break;
	case WM_RBUTTONDOWN:
		dx_input->wm_mousedown(2);
		SetCapture( hwnd );
		break;
	case WM_RBUTTONUP:
		dx_input->wm_mouseup(2);
		if( !(wparam&MK_ALLBUTTONS) ) ReleaseCapture();
		break;
	case WM_MBUTTONDOWN:
		dx_input->wm_mousedown(3);
		SetCapture( hwnd );
		break;
	case WM_MBUTTONUP:
		dx_input->wm_mouseup(3);
		if( !(wparam&MK_ALLBUTTONS) ) ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		if( !graphics ) break;
		if( gfx_mode==3 && !use_di ){
			POINT p;GetCursorPos( &p );
			dx_input->wm_mousemove( p.x,p.y );
		}else{
			int x=(short)(lparam&0xffff),y=lparam>>16;
			if( gfx_mode==1 ){
				RECT rect;GetClientRect( hwnd,&rect );
				x=x*graphics->getWidth()/(rect.right-rect.left);
				y=y*graphics->getHeight()/(rect.bottom-rect.top);
			}
			if( x<0 ) x=0;
			else if( x>=graphics->getWidth() ) x=graphics->getWidth()-1;
			if( y<0 ) y=0;
			else if( y>=graphics->getHeight() ) y=graphics->getHeight()-1;
			dx_input->wm_mousemove( x,y );
		}
		break;
	case WM_MOUSEWHEEL:
		dx_input->wm_mousewheel( (short)HIWORD( wparam ) );
		break;
	case WM_KEYDOWN:case WM_SYSKEYDOWN:
		if( lparam & 0x40000000 ) break;
		if( int n=((lparam>>17)&0x80)|((lparam>>16)&0x7f) ) dx_input->wm_keydown( n );
		break;
	case WM_KEYUP:case WM_SYSKEYUP:
		if( int n=((lparam>>17)&0x80)|((lparam>>16)&0x7f) ) dx_input->wm_keyup( n );
		break;
	default:
		return DefWindowProc( hwnd,msg,wparam,lparam );
	}

	return 0;
}

static LRESULT CALLBACK windowProc( HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam ){
	if( runtime ) return runtime->windowProc( hwnd,msg,wparam,lparam );
	return DefWindowProc( hwnd,msg,wparam,lparam );
}

//////////////////////////////
//STOP FROM EXTERNAL SOURCE //
//////////////////////////////
void gxRuntime::asyncStop(){
	PostMessage( Frame::hwnd,WM_STOP,0,0 );
}

//////////////////////////////
//RUN  FROM EXTERNAL SOURCE //
//////////////////////////////
void gxRuntime::asyncRun(){
	PostMessage( Frame::hwnd,WM_RUN,0,0 );
}

//////////////////////////////
// END FROM EXTERNAL SOURCE //
//////////////////////////////
void gxRuntime::asyncEnd(){
	PostMessage( Frame::hwnd,WM_END,0,0 );
}

//////////
// IDLE //
//////////
bool gxRuntime::idle(){
	for(;;){
		MSG msg;
		if( suspended && run_flag ){
			GetMessage( &msg,0,0,0 );
		}else{
			if( !PeekMessage( &msg,0,0,0,PM_REMOVE ) ) return run_flag;
		}
		switch( msg.message ){
		case WM_STOP:
			if( !suspended ) forceSuspend();
			break;
		case WM_RUN:
			if( suspended ) forceResume();
			break;
		case WM_END:
			bbAttachDebugger( (debugger=0) );
			run_flag=false;
			break;
		default:
			DispatchMessage( &msg );
		}
	}
	return run_flag;
}

///////////////
// DEBUGSTOP //
///////////////
void gxRuntime::debugStop(){
	if( !suspended ) forceSuspend();
}


////////////////
// DEBUGERROR //
////////////////
void gxRuntime::debugError( const char *t ){
	if( !debugger ) return;
	Debugger *d=debugger;
	asyncEnd();
	if( !suspended ){
		forceSuspend();
	}
	d->debugMsg( t,true );
}

///////////////
// DEBUGINFO //
///////////////
void gxRuntime::debugInfo( const char *t ){
	if( !debugger ) return;
	Debugger *d=debugger;
	asyncEnd();
	if( !suspended ){
		forceSuspend();
	}
	d->debugMsg( t,false );
}

/////////////////////
// POINTER VISIBLE //
/////////////////////
void gxRuntime::setPointerVisible( bool vis ){
	if( pointer_visible==vis ) return;

	pointer_visible=vis;
	if( gfx_mode==3 ) return;

	//force a WM_SETCURSOR
	POINT pt;
	GetCursorPos( &pt );
	SetCursorPos( pt.x,pt.y );
}

BBGraphics *gxRuntime::openGraphics( int w,int h,int d,int driver,int flags ){
	if( graphics ) return 0;

	busy=true;

	bool d3d=flags & BBGraphics::GRAPHICS_3D ? true : false;
	bool windowed=flags & BBGraphics::GRAPHICS_WINDOWED ? true : false;

	if( windowed ) driver=0;

	curr_driver=drivers[driver];

	if( windowed ){
		if( graphics=openWindowedGraphics( w,h,d,d3d ) ){
			gfx_mode=(flags & BBGraphics::GRAPHICS_SCALED) ? 1 : 2;
			auto_suspend=(flags & BBGraphics::GRAPHICS_AUTOSUSPEND) ? true : false;
			resize( w,h );
		}
	}else{
		backupWindowState();
		fullscreen();
		ShowCursor( 0 );
		if( graphics=openExclusiveGraphics( w,h,d,d3d ) ){
			gfx_mode=3;
			auto_suspend=true;
			SetCursorPos(0,0);
			acquireInput();
		}else{
			ShowCursor( 1 );
			restoreWindowState();
		}
	}

	if( !graphics ){
		gxGraphics::wipeSystemProperties();
		curr_driver=0;
	}else{
		((gxGraphics*)graphics)->setSystemProperties();
	}

	gfx_lost=false;

	busy=false;

	return graphics;
}

void gxRuntime::closeGraphics( BBGraphics *g ){
	if( !graphics || graphics!=g ) return;

	auto_suspend=false;

	busy=true;

	unacquireInput();
	freeInvalidationTimer();
	if( clipper ){ clipper->Release();clipper=0; }
	if( primSurf ){ primSurf->Release();primSurf=0; }
	delete graphics;graphics=0;

	if( gfx_mode==3 ){
		ShowCursor( 1 );
		restoreWindowState();
	}
	gfx_mode=0;

	gfx_lost=false;

	busy=false;
}

bool gxRuntime::graphicsLost(){
	return gfx_lost;
}

void gxRuntime::refreshSystemProperties(){
	bbSystemProperties["apphwnd"]=itoa( (int)Frame::hwnd );
	bbSystemProperties["apphinstance"]=itoa( (int)hinst );
}

void gxRuntime::enableDirectInput( bool enable ){
	if( use_di=enable ){
		acquireInput();
	}else{
		unacquireInput();
	}
}
