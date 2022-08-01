#include "linker_lld.h"
#include "../package/package.h"
#include <lld/Common/Driver.h>
#include <iostream>
#include <fstream>
#include <vector>
#ifdef BB_POSIX
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifdef BB_MSVC
#define FORMAT coff
#endif

#ifdef BB_MINGW
#define FORMAT mingw
#endif

#ifdef BB_MACOS
#define FORMAT macho
#endif

#ifdef BB_LINUX
#define FORMAT elf
#endif

static
void parse_list( std::string &line,std::vector<string> &libs ){
	size_t s=0,e=line.find(";");
	while( 1 ) {
		string frag=line.substr( s,e-s );
		while( frag[0] == ' ' ) frag=frag.substr( 1 );

		if( frag.length() == 0 ) break;

		libs.push_back( frag );

		if( e==string::npos ) {
			break;
		}

		s=e+1;
		e=line.find(";",s);
	}
}

Linker_LLD::Linker_LLD( const std::string &home ):home(home){
}

void Linker_LLD::createExe( const std::string &rt,const Target &target,const std::string &mainObj,const BundleInfo &bundle,const std::string &exeFile ){
	std::string toolchain=home+"/bin/"+target.triple;
	std::string lib_dir=toolchain+"/lib";

	// TODO: sort out all the lazy strdup business below...
	std::vector<string> args,libs,systemlibs;

	string binaryPath=exeFile;

	if( bundle.enabled && target.type=="android" ){
		createApk( toolchain,bundle,target );
	}

#ifdef BB_MACOS
	if( bundle.enabled && target.type=="native"||target.type=="ios"||target.type=="ios-sim" ){
		string bundlePath=binaryPath;
		binaryPath=binaryPath.substr( 0,binaryPath.size()-4 ); // remove .app
		string bundleid=bundle.identifier;
		string appid=basename( (char*)binaryPath.c_str() );
		string apptitle=bundle.appName;
		if( apptitle=="" ){
			apptitle[0]=toupper(apptitle[0]);
		}

		mkdir( bundlePath.c_str(),0755 );
		remove( binaryPath.c_str() );

		string iconPath=home+"/cfg/bbexe.icns";
		if( target.type=="ios"||target.type=="ios-sim" ){
			iconPath=home+"/cfg/bbios.icns";
		}

		// TODO: a bit lazy...
		if( system( ("cp "+iconPath+" "+bundlePath+"/"+appid+".icns").c_str() ) ){
			exit( 1 );
		}
		for( auto &file:bundle.files ){
			string from=file.absolutePath;
			string to=file.relativePath;

			// TODO: not perfect...
			size_t pos=0;
			while( (pos=to.find("../"))!=std::string::npos ){
				to.replace( pos,3,"" );
			}

			string dir=bundlePath;
			size_t start=0,end=0;
			while( (end=to.find( "/",start ))!=std::string::npos ){
				dir+="/"+to.substr( start,end );
				mkdir( dir.c_str(),0755 );
				start=end+1;
			}

			cout<<"Copying "+file.relativePath+"..."<<endl;
			if( system( ("cp "+from+" "+bundlePath+"/"+to).c_str() ) ){
				exit( 1 );
			}
		}

		if( target.type=="ios"||target.type=="ios-sim" ){
			cout<<"Copying b3dlogo.png..."<<endl;
			if( system( ("cp "+home+"/cfg/b3dlogo.png "+bundlePath+"/launch-logo.png").c_str() ) ){
				exit( 1 );
			}

			cout<<"Generating launch storyboard..."<<endl;
			system( ("ibtool --compile '"+bundlePath+"/launch.storyboardc' '"+home+"/cfg/b3dsplash.storyboard'").c_str() );
		}

		ofstream plist;
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
		if( target.type=="ios"||target.type=="ios-sim" ){
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

		ofstream pkginfo;
		pkginfo.open( bundlePath+"/PkgInfo");
		pkginfo << "APPL????";
		pkginfo.close();

		binaryPath=bundlePath+"/"+appid;
	}
#endif

	// just the name?
	args.push_back( "linker" );

	// TODO: should use find it via the environment...
	string ndkroot="/opt/android-sdk/ndk-bundle/toolchains/llvm/prebuilt/linux-x86_64";
	string sysroot=ndkroot+"/sysroot";

#ifdef BB_POSIX
	args.push_back("--error-limit=0");
	// args.push_back("--lto-O0");
#endif

	if( target.type=="native" ){
#ifdef BB_WINDOWS
		args.push_back( "/errorlimit:0" );

		// args.push_back( "/subsystem:windows" );

		args.push_back( "/nodefaultlib" );
		args.push_back( "oldnames.lib" );
		string winsdk="C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0";
		string msvc="C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Tools\\MSVC\\14.32.31326";
		args.push_back( "/libpath:"+winsdk+"\\ucrt\\x64" );
		args.push_back( "/libpath:"+winsdk+"\\um\\x64" );
		args.push_back( "/libpath:"+msvc+"\\lib\\x64" );

		args.push_back( "libucrt.lib" );
		args.push_back( "libvcruntime.lib" );
		args.push_back( "libcmt.lib" );
		args.push_back( "libcpmt.lib" );

		// args.push_back( "ucrt.lib" );
		// args.push_back( "vcruntime.lib" );
		// args.push_back( "msvcrt.lib" );
		// args.push_back( "msvcprt.lib" );
#endif
	}else if( target.type=="android" ){
		args.push_back("-m");args.push_back("aarch64linux");

		args.push_back("-shared");
		args.push_back("-u");args.push_back("SDL_main");

		args.push_back("-z");args.push_back("noexecstack");
		args.push_back("-EL");
		args.push_back("--fix-cortex-a53-843419");
		args.push_back("--warn-shared-textrel");
		args.push_back("-z");args.push_back("now");
		args.push_back("-z");args.push_back("relro");
		args.push_back("-z");
		args.push_back("max-page-size=4096");
		args.push_back("--hash-style=both");
		args.push_back("--enable-new-dtags");
		args.push_back("--eh-frame-hdr");

		args.push_back( ndkroot+"/sysroot/usr/lib/aarch64-linux-android/"+target.version+"/crtbegin_so.o");
		args.push_back("-L");args.push_back( ndkroot+"/lib64/clang/11.0.5/lib/linux/aarch64");
		args.push_back("-L");args.push_back( ndkroot+"/lib/gcc/aarch64-linux-android/4.9.x");
		args.push_back("-L");args.push_back( ndkroot+"/aarch64-linux-android/lib64");
		args.push_back("-L");args.push_back( ndkroot+"/aarch64-linux-android/lib");
		args.push_back("-L");args.push_back( ndkroot+"/sysroot/usr/lib/aarch64-linux-android/"+target.version);
		args.push_back("-L");args.push_back( ndkroot+"/sysroot/usr/lib/aarch64-linux-android");
		args.push_back("-L");args.push_back( ndkroot+"/sysroot/usr/lib");
	}

#ifdef BB_MACOS
	if( target.type=="native"||target.type=="ios"||target.type=="ios-sim" ){
		std::string sdkname;
		std::string platform_id,platform_version_min,platform_version_max;

		if( target.type=="ios" ){
			sdkname="iPhoneOS";
			platform_id="ios";
			platform_version_min="15.4";
			platform_version_max="15.4";
		}else if( target.type=="ios-sim" ){
			sdkname="iPhoneSimulator";
			platform_id="ios-simulator";
			platform_version_min=platform_version_max=target.version;
		}else{
			sdkname="MacOSX";
			platform_id="macos";
			platform_version_min=platform_version_max=target.version;
		}

		// TODO: use xcrun --show-sdk-path
		args.push_back("-syslibroot");
		args.push_back( "/Applications/Xcode.app/Contents/Developer/Platforms/"+sdkname+".platform/Developer/SDKs/"+sdkname+".sdk" );

		libs.push_back( "objc" );
		libs.push_back( "c" );
		libs.push_back( "c++" );

		libs.push_back( "System" );
		// libs.push_back("System_asan");

		args.push_back("-arch");
		args.push_back( target.arch );

		args.push_back( "-platform_version" );args.push_back( platform_id );
		args.push_back( platform_version_min );args.push_back( platform_version_max );
	}
#endif

#if defined(BB_MACOS) && defined(BB_ASAN)
	if( target.type=="native" ){
		// TODO: fix this hardcoding
		#define CLANG_LIBS "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/13.1.6/lib/darwin"

		args.push_back("-rpath");args.push_back(CLANG_LIBS);
		args.push_back(CLANG_LIBS "/libclang_rt.asan_osx_dynamic.dylib");
		args.push_back(CLANG_LIBS "/libclang_rt.osx.a");
	}
#endif

#ifdef BB_WINDOWS
	args.push_back( "/lldignoreenv" );

	string machine="/machine:X64";
	args.push_back( machine );

	string outArg="/out:"+binaryPath;
	args.push_back( outArg );
#else
	args.push_back("-o");args.push_back( binaryPath );
#endif

#ifdef BB_WINDOWS
	string libPath="/libpath:"+lib_dir;
	args.push_back( libPath );
#else
	args.push_back( "-L" );args.push_back( lib_dir );
#endif

	ifstream rti( toolchain+"/runtime."+rt+".i" );
	if (!rti.is_open()) {
		cerr<<"Cannot find interface file for runtime."<<rt<<endl;
		exit( 1 );
	}

	string line;
	vector<string> modules;
	while( getline( rti,line ) ) {
		string d="DEPS:";
		if( line.find( d )==0 ){
			line=line.substr( d.size() );
			parse_list( line,modules );
		}
	}

	string rtlib="runtime."+rt+".static";
	libs.push_back( rtlib );

	// libs.push_back( "bb."+mod );
	for( std::string mod:modules ){
		// libs.push_back( "bb."+mod );

		string line;
		ifstream iface( toolchain+"/cfg/"+mod+".i" );
		if (!iface.is_open()) {
			cerr<<"Cannot find interface file for "<<mod<<endl;
			abort();
		}

		while( getline( iface,line ) ) {
			string s="LIBS:",sl="SYSTEM_LIBS:";
			if( line.find( s )==0 ){
				line=line.substr( s.size() );
				parse_list( line,libs );
			}else if( line.find( sl )==0 ){
				line=line.substr( sl.size() );
				parse_list( line,systemlibs );
			}
		}
		iface.close();
	}

	string mainPath=string(tmpnam(0))+".o";
	std::ofstream mainFile( mainPath,std::ios_base::binary );
	mainFile.write( mainObj.c_str(),mainObj.size() );
	mainFile.flush();
	args.push_back( mainPath );

	//*------------------------

	for( auto lib:libs ){
		string arg;
#ifdef BB_WINDOWS
		arg=lib+string(".lib");
#else
		arg="-l"+string(lib);
#endif
		args.push_back( arg );
	}

	if( target.type=="native" ){
#ifdef BB_LINUX
		args.push_back("-dynamic-linker");args.push_back("/lib64/ld-linux-x86-64.so.2");

		// TODO: find out the best way to get this information from the environment
		// TODO: gcc --print-file-name=libc.a
		args.push_back("-L");args.push_back( LIBGCC_DIR );
		args.push_back("-L");args.push_back( "/lib" );
		args.push_back("-L");args.push_back( LIBARCH_DIR );
		args.push_back("-L");args.push_back( "/usr/lib" );
#endif
	}else if( target.type=="android" ){
		args.push_back("-dynamic-linker");args.push_back("/system/bin/linker64");
	}

	for( auto lib:systemlibs ){
#ifdef BB_WINDOWS
		args.push_back( lib+".lib" );
#else
		string fw="-framework";
		if( lib.find( fw )==0 ) {
			args.push_back( fw );
			args.push_back( lib.substr( fw.size()+1 ) );
		} else {
			args.push_back( "-l"+lib );
		}
#endif
	}

	if( target.type=="native" ){
#ifdef BB_LINUX
		args.push_back( LIBARCH_DIR "/crt1.o");
		args.push_back( LIBARCH_DIR "/crti.o");
		args.push_back( LIBGCC_DIR  "/crtbeginT.o");

		args.push_back("-lstdc++");
		args.push_back("--start-group");
		args.push_back("-lgcc");
		args.push_back("-lgcc_eh");
		args.push_back("-lc");
		args.push_back("--end-group");
		args.push_back(  LIBGCC_DIR "/crtend.o");
		args.push_back( LIBARCH_DIR "/crtn.o");
#endif
	}else if( target.type=="android" ){
		args.push_back("-l");args.push_back("c++_static");
		args.push_back("-l");args.push_back("c++abi");
		args.push_back("-l");args.push_back("gcc");
		args.push_back("-l");args.push_back("c");
		args.push_back("-l");args.push_back("gcc");
		args.push_back( ndkroot+"/sysroot/usr/lib/aarch64-linux-android/"+target.version+"/crtend_so.o" );
	}

	std::vector<const char *> _args;
	for( auto arg:args ){
		_args.push_back( strdup( arg.c_str() ) );
	}

	if( !lld::FORMAT::link( _args,llvm::outs(),llvm::errs(),false,false ) ){
		exit( 1 );
	};

	for( auto s:_args ){
		free( (char*)s );
	}

	remove( mainPath.c_str() );

#ifdef BB_MACOS
	if( bundle.enabled && bundle.signerId.size()>0 ){
		string options="--force --timestamp=none --sign '"+bundle.signerId+"'";
		if( bundle.teamId.size()>0 ){
			string xcentPath=string( tmpnam(0) )+".xcent";

			ofstream xcent;
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
#endif
}
