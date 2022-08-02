#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include "../bundle.h"
#include "../target.h"

void createApk( const std::string &out,const std::string &tmpdir,const std::string &home,const std::string &toolchain,const BundleInfo &bundle,const Target &target,const std::string &androidsdk );

#ifdef BB_MACOS
void createApp( const std::string &out,const std::string &home,const BundleInfo &bundle,const Target &target );
#endif

#endif
