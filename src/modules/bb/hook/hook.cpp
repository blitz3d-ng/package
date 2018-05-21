
#include <bb/blitz/blitz.h>
#include "hook.h"

BBHook::BBHook(){
}

void BBHook::add( BBHookFunc func,void *context ){
  Callback cb={ func,context };
  callbacks.push_back( cb );
}

void BBHook::run( void *data ){
  for( unsigned i=0;i<callbacks.size();i++ ){
    callbacks[i].func( data,callbacks[i].context );
  }
}


BBMODULE_DECL( hook );
BBMODULE_EMPTY( hook );
