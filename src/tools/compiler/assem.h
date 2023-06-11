
#ifndef ASSEM_H
#define ASSEM_H

#include "linker_x86/linker.h"

class Assem{
public:
	std::istream &in;
	Module *mod;
	Assem( std::istream &in,Module *mod ):in( in ),mod( mod ){}
	virtual void assemble()=0;
};

#endif
