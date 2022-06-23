#ifndef LINKER_LLD_H
#define LINKER_LLD_H

#include "../environ.h"
#include <string>

class Linker_LLD {
public:
	std::string home;
	const Environ *env;

	Linker_LLD( const std::string &home );

	void createExe( const std::string &rt,const std::string &mainObj, const std::string &exeFile );
};

#endif
