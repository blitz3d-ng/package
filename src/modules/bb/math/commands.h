#ifndef BB_MATH_COMMANDS_H
#define BB_MATH_COMMANDS_H

#include <bb/blitz/module.h>
#include "../../config/config.h"

#ifdef __cplusplus
extern "C" {
#endif

bb_float_t BBCALL bbSin( bb_float_t n );
bb_float_t BBCALL bbCos( bb_float_t n );
bb_float_t BBCALL bbTan( bb_float_t n );
bb_float_t BBCALL bbASin( bb_float_t n );
bb_float_t BBCALL bbACos( bb_float_t n );
bb_float_t BBCALL bbATan( bb_float_t n );
bb_float_t BBCALL bbHSin( bb_float_t n );
bb_float_t BBCALL bbHCos( bb_float_t n );
bb_float_t BBCALL bbHTan( bb_float_t n );
bb_float_t BBCALL bbATan2( bb_float_t n,bb_float_t t );
bb_float_t BBCALL bbSqr( bb_float_t n );
bb_float_t BBCALL bbFloor( bb_float_t n );
bb_float_t BBCALL bbCeil( bb_float_t n );
bb_float_t BBCALL bbExp( bb_float_t n );
bb_float_t BBCALL bbLog( bb_float_t n );
bb_float_t BBCALL bbLog10( bb_float_t n );
bb_float_t BBCALL bbRnd( bb_float_t from,bb_float_t to );
int BBCALL bbRand( int from,int to );
void BBCALL bbSeedRnd( int seed );
int BBCALL bbRndSeed();

#ifdef __cplusplus
}
#endif

#endif
