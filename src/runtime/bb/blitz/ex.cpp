
#include "module.h"
#include "ex.h"

extern "C" void BBCALL _bbDebugError( const char *e );

bbEx::bbEx( const char *e ):err(e){
  if( e ) _bbDebugError( e );
}
