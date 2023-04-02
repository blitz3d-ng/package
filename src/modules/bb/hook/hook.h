#ifndef HOOK_HOOK_H
#define HOOK_HOOK_H

#include <vector>

typedef void (*BBHookFunc)( void*,void* );

class BBHook{
protected:
  struct Callback{
    BBHookFunc func;
    void *context;
  };

  std::vector<Callback> callbacks;
public:
  BBHook();

  void add( BBHookFunc func,void *context );
  void remove( BBHookFunc func,void *context );
  void run( void *data );
};


#endif
