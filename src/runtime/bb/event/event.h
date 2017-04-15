#ifndef BB_EVENT_H
#define BB_EVENT_H

#include <bb/hook/hook.h>

enum {
  BBEVENT_APPTERMINATE=1,
  BBEVENT_KEYDOWN,
  BBEVENT_KEYUP,
  BBEVENT_MOUSEDOWN,
  BBEVENT_MOUSEUP,
  BBEVENT_MOUSEMOVE,
  BBEVENT_MOUSEWHEEL
};

struct BBEvent{
  int id,data,x,y;

  BBEvent(){}
  BBEvent( int id,int data ):id(id),data(data){}
  BBEvent( int id,int data,int x,int y ):id(id),data(data),x(x),y(y){}
};

extern BBHook bbOnEvent;

#endif
