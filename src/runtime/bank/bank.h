
#ifndef BBBANK_H
#define BBBANK_H

struct bbBank{
	char *data;
	int size,capacity;

	bbBank( int sz );
	virtual ~bbBank();
	void resize( int n );
};

#endif
