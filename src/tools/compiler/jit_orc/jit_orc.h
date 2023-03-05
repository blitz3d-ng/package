#ifndef JIT_H
#define JIT_H

#include "../libs.h"

typedef void (*BBMAIN)();
typedef int (*BBSTART)( int,char**,BBMAIN );

class JIT_ORC {
public:
	static int run( Runtime *runtime,const std::string &obj,const std::string &home,const std::string &rt );
};


#endif
