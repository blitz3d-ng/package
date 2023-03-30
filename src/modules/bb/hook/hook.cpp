
#include <bb/blitz/blitz.h>
#include "hook.h"

BBHook::BBHook(){
}

void BBHook::add( BBHookFunc func,void *context ){
  Callback cb={ func,context };
  callbacks.push_back( cb );
}

void BBHook::remove( BBHookFunc func,void *context ){
  for( size_t i=0;i<callbacks.size();i++ ){
    if( callbacks[i].func==func && callbacks[i].context==context ){
      callbacks.erase( callbacks.begin()+i );
      i--;
    }
  }
}

void BBHook::run( void *data ){
  for( auto cb:callbacks ){
    cb.func( data,cb.context );
  }
}

BBMODULE_EMPTY( hook );
