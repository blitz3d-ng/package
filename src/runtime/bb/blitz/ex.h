#ifndef BLITZ_EX_H
#define BLITZ_EX_H

struct bbEx{
	const char *err;
	bbEx( const char *e );
};

#define RTEX( _X_ ) throw bbEx( _X_ );

#endif
