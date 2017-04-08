
#ifndef LIBS_H
#define LIBS_H

#include <vector>

extern int linker_ver,runtime_ver;

void	initLibs();
string	quickHelp( const string &kw );
bool	isMediaFile( const string &file );

struct Runtime {
  string id,name;
};

extern std::vector<Runtime> runtimes;


#endif
