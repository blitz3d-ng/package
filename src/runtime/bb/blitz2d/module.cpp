
#include <bb/blitz/module.h>
#include "blitz2d.h"

extern FT_Library ft;

BBMODULE_CREATE( blitz2d ){
	if( FT_Init_FreeType(&ft) ) return false;

  return true;
}

BBMODULE_DESTROY( blitz2d ){
  return true;
}
