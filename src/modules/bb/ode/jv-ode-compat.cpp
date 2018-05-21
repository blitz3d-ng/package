
#include "ode.h"

// These functions provide "out-of-the-box" compatibility with existing
// JV-ODE code.

void BBCALL _odeRegisterODE( int key1,int key2,int key3,int key4 ){
  // THIS FUNCTION INTENTIONALLY LEFT BLANK
}

float BBCALL _odeGetVersion(){
  return 1.0f;
}

bb_int_t BBCALL _odeDemoTime(){
	return 1000;
}
