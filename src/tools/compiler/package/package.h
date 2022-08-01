#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include "../bundle.h"
#include "../target.h"

void createApk( const std::string &toolchain,const BundleInfo &bundle,const Target &target );
void createApp( const BundleInfo &bundle,const Target &target );

#endif
