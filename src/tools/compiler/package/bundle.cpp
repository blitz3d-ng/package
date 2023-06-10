#include "../bundle.h"

#include <iostream>

#ifdef BB_WINDOWS
#include <direct.h>
#define mkdir( p,a ) mkdir( p )
#else
#include <unistd.h>
#include <libgen.h>
#endif
#include <sys/stat.h>

void bundleFiles( const BundleInfo &bundle,const std::string &path ){
	for( auto &file:bundle.files ){
		std::string from=file.absolutePath;
		std::string to=file.relativePath;

		// TODO: not perfect...
		size_t pos=0;
		while( (pos=to.find("../"))!=std::string::npos ){
			to.replace( pos,3,"" );
		}

		std::string dir=path;
		size_t start=0,end=0;
		while( (end=to.find( "/",start ))!=std::string::npos ){
			dir+="/"+to.substr( start,end );
			mkdir( dir.c_str(),0755 );
			start=end+1;
		}

		std::cout<<"Copying "+file.relativePath+"..."<<std::endl;
		if( system( ("cp "+from+" "+path+"/"+to).c_str() ) ){
			exit( 1 );
		}
	}
}
