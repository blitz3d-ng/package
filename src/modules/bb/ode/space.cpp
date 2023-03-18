
#include "ode.h"
#include <ode/ode.h>

BBLIB bb_ptr_t BBCALL _odeHashSpaceCreate( bb_ptr_t space ){
  return (bb_ptr_t)dHashSpaceCreate( (dSpaceID)space );
}

BBLIB void BBCALL _odeSpaceDestroy( bb_ptr_t s ){
  dSpaceDestroy( (dSpaceID)s );
}
