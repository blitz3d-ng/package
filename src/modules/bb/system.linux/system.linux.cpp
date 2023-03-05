
#include "../stdutil/stdutil.h"
#include "system.linux.h"

#include <unistd.h>
#include <fontconfig/fontconfig.h>

#include <fstream>
using namespace std;

bool LinuxSystemDriver::delay( int ms ){
  usleep( ms * 1000 );
  return true;
}

bool LinuxSystemDriver::execute( const std::string &cmd ){
  return false;
}

int LinuxSystemDriver::getMilliSecs(){
	return 0;
}

int LinuxSystemDriver::getScreenWidth( int i ){
  return 0;
}

int LinuxSystemDriver::getScreenHeight( int i ){
  return 0;
}

void LinuxSystemDriver::dpiInfo( float &scale_x,float &scale_y ){
	scale_x=scale_y=1.0f;
}

bool LinuxSystemDriver::lookupFontData( const std::string &fontName,BBFontData &font ){
	static FcConfig* config=0;
	if( !config ) config=FcInitLoadConfigAndFonts();

	FcPattern* pat=FcNameParse( (const FcChar8*)fontName.c_str() );
	FcConfigSubstitute( config,pat,FcMatchPattern );
	FcDefaultSubstitute( pat );
	char* fontPath=0;
	FcResult result;
	FcPattern* match=FcFontMatch( config,pat,&result );
	if( match ){
		FcChar8* file = NULL;
		if( FcPatternGetString( match,FC_FILE,0,&file )==FcResultMatch ){
			fontPath = (char*)file;
		}
	}
	FcPatternDestroy( pat );

	if( !fontPath ) return false;

	ifstream file( fontPath,ios::binary|ios::ate );
	font.size=file.tellg();
	file.seekg( 0,ios::beg );

	font.data=new unsigned char[font.size];
	if( file.read( (char*)font.data,font.size ) ){
		return true;
	}
	return false;
}

BBMODULE_CREATE( system_linux ){
	if( !bbSystemDriver ){
		bbSystemDriver=d_new LinuxSystemDriver();
	}
	return true;
}

BBMODULE_DESTROY( system_linux ){
	return true;
}
