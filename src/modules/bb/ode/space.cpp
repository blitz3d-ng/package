
#include "ode.h"
#include <ode/ode.h>

bb_ptr_t BBCALL _odeHashSpaceCreate( bb_ptr_t space ){
  return (bb_ptr_t)dHashSpaceCreate( (dSpaceID)space );
}

void BBCALL _odeSpaceDestroy( bb_ptr_t s ){
  dSpaceDestroy( (dSpaceID)s );
}
