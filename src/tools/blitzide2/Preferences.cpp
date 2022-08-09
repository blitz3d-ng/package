#include "Preferences.h"
#include <toml.hpp>
using namespace std;

void Preferences::Load( const wxString &blitzpath ){
	path=blitzpath+"/cfg/blitzide.toml";

	ifstream ifs( path.ToStdString(),ios_base::binary );
	if( ifs.good() ){
		const toml::value data=toml::parse( ifs,path.ToStdString() );
		signId=wxString( toml::find<string>(data,"signId") );
		teamId=wxString( toml::find<string>(data,"teamId") );
	}else{
		ResetToDefaults();
	}
}

void Preferences::Save(){
	toml::value table{
		{ "signId",signId.ToStdString() },
		{ "teamId",teamId.ToStdString() }
	};

	ofstream out;
	out.open( path.ToStdString() );
	out<<table;
	out.close();
}

void Preferences::ResetToDefaults(){
	signId="";
	teamId="";
}
