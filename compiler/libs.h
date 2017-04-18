
#ifndef LIBS_H
#define LIBS_H

#include <map>
#include <list>
#include <vector>
#include <string>

using namespace std;

#include "../compiler/environ.h"
#include "../compiler/parser.h"
#ifdef WIN32
#include "../linker/linker.h"
#include "../bbruntime_dll/bbruntime_dll.h"
#endif

extern int bcc_ver;
extern int lnk_ver;
extern int run_ver;
extern int dbg_ver;

//openLibs
extern string home;
#ifdef WIN32
extern Linker *linkerLib;
extern Runtime *runtimeLib;

//linkLibs
extern Module *runtimeModule;
#endif
extern Environ *runtimeEnviron;
extern vector<string> modules;
extern vector<string> keyWords;
extern vector<UserFunc> userFuncs;

const char *openLibs( const string rt );

const char *linkLibs();

void closeLibs();

#endif
