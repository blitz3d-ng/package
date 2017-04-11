
#include "frame.h"
#include <bb/blitz/app.h>
using namespace std;

static const int static_ws=WS_VISIBLE|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
static const int scaled_ws=WS_VISIBLE|WS_CAPTION|WS_SYSMENU|WS_SIZEBOX|WS_MINIMIZEBOX|WS_MAXIMIZEBOX;

Frame::Frame( HWND hwnd ):hwnd(hwnd),gfx_mode(0){
	bbAppOnChange.add( _refreshTitle,this );
}

void Frame::_refreshTitle( void *data,void *context ){
	((Frame*)context)->setTitle( ((BBApp*)data)->title );
}

void Frame::setTitle( const string &t ){
	SetWindowText( hwnd,t.c_str() );
}

void Frame::backupWindowState(){
	GetWindowRect( hwnd,&t_rect );
	t_style=GetWindowLong( hwnd,GWL_STYLE );
}

void Frame::restoreWindowState(){
	SetWindowLong( hwnd,GWL_STYLE,t_style );
	SetWindowPos(
		hwnd,0,t_rect.left,t_rect.top,
		t_rect.right-t_rect.left,t_rect.bottom-t_rect.top,
		SWP_NOZORDER|SWP_FRAMECHANGED );
}

void Frame::resize( int w,int h ){
  if( gfx_mode==3 ) return;

  int ws,ww,hh;
  if( gfx_mode==1 ){
    ws=scaled_ws;
    RECT c_r;
    GetClientRect( hwnd,&c_r );
    ww=c_r.right-c_r.left;
    hh=c_r.bottom-c_r.top;
  }else{
    ws=static_ws;
    ww=w;
    hh=h;
  }

	SetWindowLong( hwnd,GWL_STYLE,ws );
	SetWindowPos( hwnd,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED );

	RECT w_r,c_r;
	GetWindowRect( hwnd,&w_r );
	GetClientRect( hwnd,&c_r );
	int tw=(w_r.right-w_r.left)-(c_r.right-c_r.left);
	int th=(w_r.bottom-w_r.top)-(c_r.bottom-c_r.top );
	int cx=( GetSystemMetrics( SM_CXSCREEN )-ww )/2;
	int cy=( GetSystemMetrics( SM_CYSCREEN )-hh )/2;
	POINT zz={0,0};
	ClientToScreen( hwnd,&zz );
	int bw=zz.x-w_r.left,bh=zz.y-w_r.top;
	int wx=cx-bw,wy=cy-bh;if( wy<0 ) wy=0;		//not above top!
	MoveWindow( hwnd,wx,wy,ww+tw,hh+th,true );
}

void Frame::fullscreen(){
  SetWindowLong( hwnd,GWL_STYLE,WS_VISIBLE|WS_POPUP );
  SetWindowPos( hwnd,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED );
}
