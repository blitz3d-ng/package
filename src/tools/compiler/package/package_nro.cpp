#include "package.h"
#include <iostream>
#include <libgen.h>

#define RUN( args ) if( system( std::string(args).c_str() )!=0 ) { std::cerr<<"error on "<<__FILE__<<":"<<__LINE__<<std::endl;exit(1); }

void createNRO( const std::string &out,const std::string &home,const std::string &devkitpro,const BundleInfo &bundle,const Target &target,const std::string &elfPath ){
	std::string o=out;
	std::string dir=dirname( (char*)o.c_str() );
	std::string base=basename( (char*)o.c_str() );
	base=base.substr( 0,base.size()-4 );
	std::string nacpPath=dir+"/"+base+".nacp";

	// must be a jpg? 256x256
	std::string icon=home+"/cfg/bbexe.jpg";

	RUN( devkitpro+"/tools/bin/nacptool --create \""+bundle.appName+"\" \"Unspecified Author\" \"1.0.0\" "+nacpPath );
	RUN( devkitpro+"/tools/bin/elf2nro "+elfPath+" "+out+" --icon="+icon+" --nacp="+nacpPath );
}
