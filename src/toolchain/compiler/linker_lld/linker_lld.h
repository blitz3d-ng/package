#ifndef LINKER_LLD_H
#define LINKER_LLD_H

#include <string>

class Linker_LLD {
public:
	std::string home;

	Linker_LLD( const std::string &home );

	void createExe( const std::string &exeFile );
};

#endif
