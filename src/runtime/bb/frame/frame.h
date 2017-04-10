#ifndef FRAME_FRAME_H
#define FRAME_FRAME_H

#include <windows.h>
#include <string>

class Frame{
public:
  Frame( HWND hwnd );
public: // TODO: make these protected
  HWND hwnd;

  std::string app_title;
  std::string app_close;
protected:
  //current gfx mode
  //
  //0=NONE
  //1=SCALED WINDOW
  //2=FIXED SIZE WINDOW
  //3=EXCLUSIVE
  //
  int gfx_mode;

  RECT t_rect;
  int t_style;

  void backupWindowState();
  void restoreWindowState();

  void resize( int w,int h );
  void fullscreen();
public:
  void setTitle( const std::string &title,const std::string &close );
};

#endif
