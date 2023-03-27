
#include "ode.h"

// These functions provide "out-of-the-box" compatibility with existing
// JV-ODE code.

BBLIB void BBCALL _odeRegisterODE( bb_int_t key1,bb_int_t key2,bb_int_t key3,bb_int_t key4 ){
  // THIS FUNCTION INTENTIONALLY LEFT BLANK
}

BBLIB bb_float_t BBCALL _odeGetVersion(){
  return 1.0f;
}

BBLIB bb_int_t BBCALL _odeDemoTime(){
	return 1000;
}
