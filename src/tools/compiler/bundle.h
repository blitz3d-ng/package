#ifndef BUNDLE_H
#define BUNDLE_H

#include <string>
#include <vector>

struct BundleFile {
	string relativePath;
	string absolutePath;

	BundleFile( string relpath,string abspath ):relativePath(relpath),absolutePath(abspath){};
};

struct BundleInfo {
	bool enabled=false; // bundle this app?
	string identifier; // platform specific ID
	string appName; // label used wherever a human readable title is needed
	vector<BundleFile> files; // files to copy into the bundle
	string signerId,teamId;
};

#endif
