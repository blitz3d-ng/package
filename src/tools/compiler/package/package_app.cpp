#include "package.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>

#define RUN( args ) if( system( std::string(args).c_str() )!=0 ) { cerr<<"error on "<<__FILE__<<":"<<__LINE__<<endl;abort(); }

void createApp( const std::string &bundlePath,const std::string &home,const BundleInfo &bundle,const Target &target ){
	const bool ios=target.type=="ios"||target.type=="ios-sim";

	std::string binaryPath=bundlePath.substr( 0,bundlePath.size()-4 ); // remove .app
	std::string bundleid=bundle.identifier;
	std::string appid=basename( (char*)binaryPath.c_str() );
	std::string apptitle=bundle.appName;
	binaryPath=bundlePath+"/"+appid;
	if( apptitle=="" ){
		apptitle[0]=toupper(apptitle[0]);
	}

	mkdir( bundlePath.c_str(),0755 );

	std::string iconPath=home+"/cfg/bbexe.icns";
	if( target.type=="ios"||target.type=="ios-sim" ){
		iconPath=home+"/cfg/bbios.icns";
	}

	// TODO: a bit lazy...
	if( system( ("cp "+iconPath+" "+bundlePath+"/"+appid+".icns").c_str() ) ){
		exit( 1 );
	}

	bundleFiles( bundle,bundlePath );

	if( ios ){
		std::cout<<"Copying b3dlogo.png..."<<std::endl;
		if( system( ("cp "+home+"/cfg/b3dlogo.png "+bundlePath+"/launch-logo.png").c_str() ) ){
			exit( 1 );
		}

		std::cout<<"Generating launch storyboard..."<<std::endl;
		system( ("ibtool --compile '"+bundlePath+"/launch.storyboardc' '"+home+"/cfg/b3dsplash.storyboard'").c_str() );
	}

	std::ofstream plist;
	plist.open( bundlePath+"/Info.plist");
	plist << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	plist << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
	plist << "<plist version=\"1.0\">\n";
	plist << "<dict>\n";
	plist << "  <key>CFBundleName</key>\n";
	plist << "  <string>"<<apptitle<<"</string>\n";
	plist << "  <key>CFBundleDisplayName</key>\n";
	plist << "  <string>"<<apptitle<<"</string>\n";
	plist << "  <key>CFBundleExecutable</key>\n";
	plist << "  <string>"<<appid<<"</string>\n";
	plist << "  <key>CFBundleIconFile</key>\n";
	plist << "  <string>"<<appid<<".icns</string>\n";
	plist << "  <key>CFBundlePackageType</key>\n";
	plist << "  <string>APPL</string>\n";
	plist << "  <key>CFBundleIdentifier</key>\n";
	plist << "  <string>"<<bundleid<<"</string>\n";
	plist << "  <key>CFBundleInfoDictionaryVersion</key>\n";
	plist << "  <string>6.0</string>\n";
	plist << "  <key>CFBundleShortVersionString</key>\n";
	plist << "  <string>1.0.0</string>\n";
	plist << "  <key>CFBundleSignature</key>\n";
	plist << "  <string>\?\?\?\?</string>\n";
	plist << "  <key>CFBundleVersion</key>\n";
	plist << "  <string>1.0</string>\n";
	plist << "  <key>NSMainNibFile</key>\n";
	plist << "  <string></string>\n";
	plist << "  <key>UIApplicationSupportsIndirectInputEvents</key>\n";
	plist << "  <true/>\n";
	if( ios ){
		plist << "  <key>UILaunchStoryboardName</key>\n";
		plist << "  <string>launch</string>\n";
		plist << "  <key>UISupportedInterfaceOrientations</key>\n";
		plist << "  <array>\n";
		plist << "    <string>UIInterfaceOrientationPortrait</string>\n";
		plist << "    <string>UIInterfaceOrientationPortraitUpsideDown</string>\n";
		plist << "    <string>UIInterfaceOrientationLandscapeLeft</string>\n";
		plist << "    <string>UIInterfaceOrientationLandscapeRight</string>\n";
		plist << "  </array>\n";
		plist << "  <key>UIDeviceFamily</key>\n";
		plist << "  <array>\n";
		plist << "    <string>1</string>\n";
		plist << "    <string>2</string>\n";
		plist << "  </array>\n";
	}
	plist << "</dict>\n";
	plist << "</plist>\n";

	plist.close();

	std::ofstream pkginfo;
	pkginfo.open( bundlePath+"/PkgInfo");
	pkginfo << "APPL????";
	pkginfo.close();

	if( bundle.signerId.size()>0 ){
		std::cout<<"Signing "+bundle.identifier+"..."<<std::endl;

		std::string options="--force --timestamp=none --sign '"+bundle.signerId+"'";
		if( bundle.teamId.size()>0 ){
			std::string xcentPath=std::string( tmpnam(0) )+".xcent";

			std::ofstream xcent;
			xcent.open( xcentPath );
			xcent<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			xcent<<"<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
			xcent<<"<plist version=\"1.0\">\n";
			xcent<<"<dict>\n";
			xcent<<"  <key>application-identifier</key>\n";
			xcent<<"  <string>"<<bundle.teamId<<"."<<bundle.identifier<<"</string>\n";
			xcent<<"  <key>com.apple.developer.team-identifier</key>\n";
			xcent<<"  <string>"<<bundle.teamId<<"</string>\n";
			xcent<<"</dict>\n";
			xcent<<"</plist>\n";
			xcent.close();

			options +=" --entitlements "+xcentPath;
		}

		system( ("codesign "+options+" "+binaryPath).c_str() );
	}
}
