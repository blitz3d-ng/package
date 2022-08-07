#include "Preferences.h"
#include <toml.hpp>
using namespace std;

void Preferences::Load( const wxString &blitzpath ){
	string path=(blitzpath+"/cfg/blitzide.toml").ToStdString();
	ifstream ifs( path,ios_base::binary );
	if( ifs.good() ){
		const toml::value data=toml::parse( ifs,path );
		signId=wxString( toml::find<string>(data,"signId") );
		teamId=wxString( toml::find<string>(data,"teamId") );
	}else{
		ResetToDefaults();
	}
}

void Preferences::Save( const wxString &blitzpath ){
	string path=(blitzpath+"/cfg/blitzide.toml").ToStdString();

	toml::value table{
		{ "signId",signId.ToStdString() },
		{ "teamId",teamId.ToStdString() }
	};

	ofstream out;
	out.open( path );
	out<<table;
	out.close();
}

void Preferences::ResetToDefaults(){
	signId="";
	teamId="";
}
