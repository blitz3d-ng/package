
#ifndef BBMATH_H
#define BBMATH_H

#include "../blitz/module.h"

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
void	 BBCALL bbSeedRnd( int seed );

#endif
