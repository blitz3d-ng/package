#ifndef LINKER_LLD_H
#define LINKER_LLD_H

#include <string>

class Linker_LLD {
public:
	std::string home;
	std::string rt;

	Linker_LLD( const std::string &home, const std::string &rt );

	void createExe( const std::string &mainObj, const std::string &exeFile );
};

#endif
