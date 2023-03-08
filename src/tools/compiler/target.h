#ifndef TARGET_H
#define TARGET_H

#include <string>
#include <vector>
#include <map>

struct Target{
	struct Runtime{
		std::string id,entry;
		std::vector<std::string> modules;
	};

	struct Module{
		std::string id;
		std::vector<std::string> dependencies,libs,system_libs,extra_files,symbols;
	};

	bool host;
	std::string triple,type;
	std::string arch,version;
	std::map<std::string,Runtime> runtimes;
	std::map<std::string,Module> modules;

	Target( std::string triple,std::string type,std::string arch,std::string version ):triple(triple),type(type),arch(arch),version(version){
		host=(triple==BB_TRIPLE);
	}
};

#endif
