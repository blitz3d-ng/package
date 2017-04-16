#ifndef BB_RUNTIME_RUNTIME_H
#define BB_RUNTIME_RUNTIME_H

#include <bb/blitz/module.h>
#include <bb/hook/hook.h>

class BBRuntime{
public:
  virtual void asyncStop()=0;
  virtual void asyncRun()=0;
  virtual void asyncEnd()=0;

  virtual bool idle()=0;

  virtual void *window()=0;

  virtual void moveMouse( int x,int y )=0;
  virtual void setPointerVisible( bool vis )=0;
};

void BBCALL bbShowPointer();
void BBCALL bbHidePointer();

void *bbRuntimeWindow();
bool bbRuntimeIdle();

extern BBRuntime *bbRuntime;
extern BBHook *bbRuntimeOnSuspend;
extern BBHook *bbRuntimeOnResume;

BBRuntime *bbCreateRuntime();

#endif
