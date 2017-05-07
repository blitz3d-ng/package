#ifndef BB_MATH_COMMANDS_H
#define BB_MATH_COMMANDS_H

#include <bb/blitz/module.h>

#ifdef __cplusplus
extern "C" {
#endif

float	 BBCALL bbSin( float n );
float	 BBCALL bbCos( float n );
float	 BBCALL bbTan( float n );
float	 BBCALL bbASin( float n );
float	 BBCALL bbACos( float n );
float	 BBCALL bbATan( float n );
float	 BBCALL bbHSin( float n );
float	 BBCALL bbHCos( float n );
float	 BBCALL bbHTan( float n );
float	 BBCALL bbATan2( float n,float t );
float	 BBCALL bbSqr( float n );
float	 BBCALL bbFloor( float n );
float	 BBCALL bbCeil( float n );
float	 BBCALL bbExp( float n );
float	 BBCALL bbLog( float n );
float	 BBCALL bbLog10( float n );
float	 BBCALL bbRnd( float from,float to );
int	 BBCALL bbRand( int from,int to );
void	 BBCALL bbSeedRnd( int seed );
int BBCALL bbRndSeed();

#ifdef __cplusplus
}
#endif

#endif
