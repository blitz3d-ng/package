#ifndef BUNDLE_H
#define BUNDLE_H

#include <string>
#include <vector>

struct BundleFile {
	std::string relativePath;
	std::string absolutePath;

	BundleFile( std::string relpath,std::string abspath ):relativePath(relpath),absolutePath(abspath){};
};

struct BundleInfo {
	bool enabled=false; // bundle this app?
	std::string identifier; // platform specific ID
	std::string appName; // label used wherever a human readable title is needed
	std::vector<BundleFile> files; // files to copy into the bundle
	std::string signerId,teamId;
};

#endif
