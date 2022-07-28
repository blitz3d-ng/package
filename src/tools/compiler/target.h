#ifndef TARGET_H
#define TARGET_H

#include <string>
#include <vector>

struct Target{
	std::string triple,type;
	std::string arch,version;
	std::vector<std::string> rts;

	Target( std::string triple,std::string type,std::string arch,std::string version ):triple(triple),type(type),arch(arch),version(version){}
};

#endif
