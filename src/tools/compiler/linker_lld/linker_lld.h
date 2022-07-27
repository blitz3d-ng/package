#ifndef LINKER_LLD_H
#define LINKER_LLD_H

#include "../environ.h"
#include "../bundle.h"
#include <string>

class Linker_LLD {
public:
	std::string home;
	const Environ *env;

	Linker_LLD( const std::string &home );

	void createExe( const std::string &rt,const std::string &target,const std::string &triple,const std::string &mainObj,const BundleInfo &bundle,const std::string &exeFile );
};

#endif
