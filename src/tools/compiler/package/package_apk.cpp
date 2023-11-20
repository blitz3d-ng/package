#include "package.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#ifdef BB_WINDOWS
#include <io.h>
#include <direct.h>
#include <stdlib.h>
#define PATH_MAX _MAX_PATH
#define getwd( buff ) _getcwd( buff,_MAX_PATH )
#define access( a,b ) (_access( a,b )==0)
#define chdir _chdir
#define F_OK 00
#define FILE_EXISTS 0
#else
#include <unistd.h>
#define FILE_EXISTS
#endif
#include <limits.h>

#define RUN( args ) if( system( std::string(args).c_str() )!=0 ) { std::cerr<<"error on "<<__FILE__<<":"<<__LINE__<<std::endl;abort(); }

void createApk( const std::string &out,const std::string &tmpdir,const std::string &home,const std::string &toolchain,const BundleInfo &bundle,const Target &target,const std::string &rt,const std::string &androidsdk ){
	std::string libdir=toolchain+"/lib";

	std::string btversion="30.0.2";
	std::string buildtools=androidsdk+"/build-tools/"+btversion;
	std::string aapt=buildtools+"/aapt";
	std::string aapt2=buildtools+"/aapt2";
	std::string dex=buildtools+"/dx";
	std::string apksigner=buildtools+"/apksigner";
	std::string zipalign=buildtools+"/zipalign";
	std::string androidjar=androidsdk+"/platforms/android-"+target.version+"/android.jar";

	// TODO: support release keys...
	std::string keystore=home+"/cfg/debug.keystore";
	std::string keystorepass="pass:android";

	std::string manifest=tmpdir+"/AndroidManifest.xml";

	std::string resdir=tmpdir+"/res";

	RUN( "mkdir -p "+resdir );

	// icons...
	RUN( "mkdir -p "+resdir+"/mipmap-hdpi" );
	RUN( "mkdir -p "+resdir+"/mipmap-mdpi" );
	RUN( "mkdir -p "+resdir+"/mipmap-xhdpi" );
	RUN( "mkdir -p "+resdir+"/mipmap-xxhdpi" );
	RUN( "mkdir -p "+resdir+"/mipmap-xxxhdpi" );

	RUN( "cp "+home+"/cfg/bbexe.png "+resdir+"/mipmap-hdpi/ic_launcher.png" );
	RUN( "cp "+home+"/cfg/bbexe.png "+resdir+"/mipmap-mdpi/ic_launcher.png" );
	RUN( "cp "+home+"/cfg/bbexe.png "+resdir+"/mipmap-xhdpi/ic_launcher.png" );
	RUN( "cp "+home+"/cfg/bbexe.png "+resdir+"/mipmap-xxhdpi/ic_launcher.png" );
	RUN( "cp "+home+"/cfg/bbexe.png "+resdir+"/mipmap-xxxhdpi/ic_launcher.png" );

	// values...
	RUN( "mkdir -p "+resdir+"/values" );

	std::ofstream colors;
	colors.open( resdir+"/values/colors.xml" );
	colors<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	colors<<"<resources>\n";
	colors<<"  <color name=\"colorPrimary\">#3F51B5</color>\n";
	colors<<"  <color name=\"colorPrimaryDark\">#303F9F</color>\n";
	colors<<"  <color name=\"colorAccent\">#FF4081</color>\n";
	colors<<"</resources>\n";
	colors.close();

	std::ofstream strings;
	strings.open( resdir+"/values/strings.xml" );
	strings<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	strings<<"<resources>\n";
	strings<<"    <string name=\"app_name\">"+bundle.appName+"</string>\n";
	strings<<"</resources>\n";
	strings.close();

	std::ofstream styles;
	styles.open( resdir+"/values/styles.xml" );
	styles<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	styles<<"<resources>\n";
	styles<<"  <style name=\"AppTheme\" parent=\"android:Theme.Holo.Light.DarkActionBar\">\n";
	styles<<"  </style>\n";
	styles<<"</resources>\n";
	styles.close();

	// manifest...
	std::ofstream m;
	m.open( manifest );
	m<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	m<<"<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"\n";
	m<<"  package=\""+bundle.identifier+"\"\n";
	m<<"  android:versionCode=\"1\"\n";
	m<<"  android:versionName=\"1.0\"\n";
	m<<"  android:installLocation=\"auto\">\n";
	m<<"\n";
	m<<"  <uses-sdk android:minSdkVersion=\""+target.version+"\"\n";
	m<<"            android:targetSdkVersion=\""+target.version+"\" />\n";
	m<<"\n";
	std::ifstream content( toolchain+"/manifest.template.xml" );
	if( content.is_open() ){
		std::string line;
		while( getline( content,line ) ){
			m<<"  "<<line<<'\n';
		}
		content.close();
	}
	m<<"</manifest>\n";
	m.close();

	// assets...
	RUN( "mkdir -p "+tmpdir+"/assets" );
	bundleFiles( bundle,tmpdir+"/assets" );

	// build the apk...
	std::string jars="";

	const Target::Runtime &rti=target.runtimes.at( rt );
	for( std::string mod:rti.modules ){
		const Target::Module &m=target.modules.at( mod );
		for( std::string lib:m.extra_files ){
			if( lib.substr( std::max( 4,(int)lib.size() )-4 )==".jar" ){
				jars=jars+" "+libdir+"/"+lib;
			}
		}
	}

	if( jars!="" ){
		RUN( dex+" --dex --output="+tmpdir+"/classes.dex "+jars );
	}

	for( std::string mod:rti.modules ){
		const Target::Module &m=target.modules.at( mod );
		for( std::string lib:m.extra_files ){
			if( lib.substr( std::max( 3,(int)lib.size() )-3 )==".so" ){
				RUN( "cp "+libdir+"/"+lib+" "+tmpdir+"/lib/"+target.arch+"/" );
			}
		}
	}

	RUN( aapt2+" compile -v --dir "+resdir+" -o "+tmpdir+"/resources.zip" );
	RUN( aapt2+" link -v -o "+tmpdir+"/unaligned.apk -I "+androidjar+" --manifest "+manifest+" -A "+tmpdir+"/assets "+tmpdir+"/resources.zip" );

	// since relative paths create issues...
	char dir[PATH_MAX];
	getcwd( dir,PATH_MAX );
	std::string currdir=dir;
	chdir( tmpdir.c_str() );
	RUN( "zip -u unaligned.apk *.dex lib/**/*.so" );
	chdir( currdir.c_str() );

	RUN( (zipalign+" -f 4 "+tmpdir+"/unaligned.apk "+tmpdir+"/signed.apk").c_str() );

	if( access( keystore.c_str(),F_OK ) ) {
		RUN( ("keytool -genkey -v -keystore "+keystore+" -storepass android -alias androiddebugkey -keypass android -keyalg RSA -keysize 2048 -validity 10000 -dname \"C=US, O=Android, CN=Android Debug\"" ).c_str() );
	}
	RUN( (apksigner+" sign --ks "+keystore+" --ks-pass "+keystorepass+" "+tmpdir+"/signed.apk").c_str() );

	RUN( ("cp "+tmpdir+"/signed.apk "+out).c_str() );
}
