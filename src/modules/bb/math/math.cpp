
#include <cmath>
#include <algorithm>

#include "math.h"

static int rnd_state;
static const int RND_A=48271;
static const int RND_M=2147483647;
static const int RND_Q=44488;
static const int RND_R=3399;

static const bb_float_t dtor=0.0174532925199432957692369076848861f;
static const bb_float_t rtod=57.2957795130823208767981548141052f;

bb_float_t BBCALL bbSin( bb_float_t n ){ return (bb_float_t)sin(n*dtor); }
bb_float_t BBCALL bbCos( bb_float_t n ){ return (bb_float_t)cos(n*dtor); }
bb_float_t BBCALL bbTan( bb_float_t n ){ return (bb_float_t)tan(n*dtor); }
bb_float_t BBCALL bbASin( bb_float_t n ){ return (bb_float_t)asin(n)*rtod; }
bb_float_t BBCALL bbACos( bb_float_t n ){ return (bb_float_t)acos(n)*rtod; }
bb_float_t BBCALL bbATan( bb_float_t n ){ return (bb_float_t)atan(n)*rtod; }
bb_float_t BBCALL bbATan2( bb_float_t n,bb_float_t t ){ return (bb_float_t)atan2(n,t)*rtod; }
bb_float_t BBCALL bbSqr( bb_float_t n ){ return (bb_float_t)sqrt(n); }
bb_float_t BBCALL bbFloor( bb_float_t n ){ return (bb_float_t)floor(n); }
bb_float_t BBCALL bbCeil( bb_float_t n ){ return (bb_float_t)ceil(n); }
bb_float_t BBCALL bbExp( bb_float_t n ){ return (bb_float_t)exp(n); }
bb_float_t BBCALL bbLog( bb_float_t n ){ return (bb_float_t)log(n); }
bb_float_t BBCALL bbLog10( bb_float_t n ){ return (bb_float_t)log10(n); }

//return rand float from 0...1
static inline bb_float_t rnd(){
	rnd_state=RND_A*(rnd_state%RND_Q)-RND_R*(rnd_state/RND_Q);
	if( rnd_state<0 ) rnd_state+=RND_M;
	return (bb_float_t)(rnd_state&65535)/65536.0+(.5/65536.0);
}

bb_float_t BBCALL bbRnd( bb_float_t from,bb_float_t to ){
	return rnd()*(to-from)+from;
}

bb_int_t BBCALL bbRand( bb_int_t from,bb_int_t to ){
	if( to<from ) std::swap( from,to );
	return int(rnd()*(to-from+1))+from;
}

void BBCALL bbSeedRnd( bb_int_t seed ){
	seed&=0x7fffffff;
	rnd_state=seed ? seed : 1;
}

bb_int_t  BBCALL bbRndSeed(){
	return rnd_state;
}

// math module entrypoints

BBMODULE_CREATE( math ){
	bbSeedRnd( 0x1234 );
	return true;
}

BBMODULE_DESTROY( math ){
	return true;
}
