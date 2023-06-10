
#ifndef LIBS_H
#define LIBS_H

#include <map>
#include <list>
#include <vector>
#include <string>

#include "environ.h"
#include "tree/parser.h"
#include "linker_x86/linker.h"
#include "../../runtime/base/bbruntime_dll.h"
#include "target.h"

extern int bcc_ver;
extern int lnk_ver;
extern int run_ver;
extern int dbg_ver;

//openLibs
extern std::string home;
extern Linker *linkerLib;
extern Runtime *runtimeLib;

extern BBRuntimeDylib rtdl;

//linkLibs
extern Module *runtimeModule;
extern Environ *runtimeEnviron;
extern std::vector<std::string> modules;
extern std::vector<std::string> keyWords;
extern std::vector<UserFunc> userFuncs;

const char *openLibs( const Target &t,const std::string &rt );

const char *linkLibs( const Target &t,const std::string &rt );

void closeLibs();

#endif
