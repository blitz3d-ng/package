
#ifndef BBBANK_H
#define BBBANK_H

#include <bb/stream/stream.h>

struct bbBank{
	char *data;
	int size,capacity;

	bbBank( int sz );
	virtual ~bbBank();
	void resize( int n );
};

#include "commands.h"

#endif
