
#include "ode.h"

// These functions provide "out-of-the-box" compatibility with existing
// JV-ODE code.

void BBCALL _odeRegisterODE( bb_int_t key1,bb_int_t key2,bb_int_t key3,bb_int_t key4 ){
  // THIS FUNCTION INTENTIONALLY LEFT BLANK
}

bb_float_t BBCALL _odeGetVersion(){
  return 1.0f;
}

bb_int_t BBCALL _odeDemoTime(){
	return 1000;
}
