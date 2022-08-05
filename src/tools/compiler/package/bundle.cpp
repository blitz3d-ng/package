#include "../bundle.h"

#include <iostream>
using namespace std;

#ifdef BB_WINDOWS
#include <direct.h>
#define mkdir( p,a ) mkdir( p )
#else
#include <unistd.h>
#include <libgen.h>
#endif
#include <sys/stat.h>

void bundleFiles( const BundleInfo &bundle,const string &path ){
	for( auto &file:bundle.files ){
		string from=file.absolutePath;
		string to=file.relativePath;

		// TODO: not perfect...
		size_t pos=0;
		while( (pos=to.find("../"))!=std::string::npos ){
			to.replace( pos,3,"" );
		}

		string dir=path;
		size_t start=0,end=0;
		while( (end=to.find( "/",start ))!=std::string::npos ){
			dir+="/"+to.substr( start,end );
			mkdir( dir.c_str(),0755 );
			start=end+1;
		}

		cout<<"Copying "+file.relativePath+"..."<<endl;
		if( system( ("cp "+from+" "+path+"/"+to).c_str() ) ){
			exit( 1 );
		}
	}
}
