
#ifndef LIBS_H
#define LIBS_H

#include <vector>

extern int linker_ver,runtime_ver;

void	initLibs();
std::string	quickHelp( const std::string &kw );
bool	isMediaFile( const std::string &file );

struct Runtime {
  std::string id,name;
};

extern std::vector<Runtime> runtimes;


#endif
