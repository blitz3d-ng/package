
#include "ex.h"
#include "debug.h"

bbEx::bbEx( const char *e ):err(e){
  if( e ) _bbDebugError( e );
}
