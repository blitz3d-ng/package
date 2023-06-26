#ifndef BLITZ_EX_H
#define BLITZ_EX_H

#include <string>

struct bbEx{
	std::string err;
	bbEx( const char *e );
	~bbEx();
};

#define RTEX( _X_ ) throw bbEx( _X_ );

#endif
