#include "Preferences.h"
#include <toml.hpp>

void Preferences::Load( const wxString &blitzpath ){
	path=blitzpath+"/cfg/blitzide.toml";

	std::ifstream ifs( path.ToStdString(),std::ios_base::binary );
	if( ifs.good() ){
		const toml::value data=toml::parse( ifs,path.ToStdString() );
		signId=wxString( toml::find<std::string>(data,"signId") );
		teamId=wxString( toml::find<std::string>(data,"teamId") );
	}else{
		ResetToDefaults();
	}
}

void Preferences::Save(){
	toml::value table{
		{ "signId",signId.ToStdString() },
		{ "teamId",teamId.ToStdString() }
	};

	std::ofstream out;
	out.open( path.ToStdString() );
	out<<table;
	out.close();
}

void Preferences::ResetToDefaults(){
	signId="";
	teamId="";
}
