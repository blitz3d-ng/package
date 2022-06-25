#include "linker_lld.h"
#include <lld/Common/Driver.h>
#include <iostream>
#include <fstream>
#include <vector>
#ifdef BB_POSIX
#include <libgen.h>
#include <sys/stat.h>
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

Linker_LLD::Linker_LLD( const std::string &home ):home(home){
}

void Linker_LLD::createExe( const std::string &rt,const std::string &mainObj,const BundleInfo &bundle,const std::string &exeFile ){
	std::string toolchain=home+"/toolchains/" BB_TRIPLE;
	std::string lib_dir=toolchain+"/lib";

	std::vector<const char *> args;

	string binaryPath=exeFile;

#ifdef BB_MACOS
	if( bundle.enabled ){
		string bundlePath=binaryPath;
		binaryPath=binaryPath.substr( 0,binaryPath.size()-4 ); // remove .app
		string appid=basename( (char*)binaryPath.c_str() );
		string apptitle=bundle.appName;
		if( apptitle=="" ){
			apptitle[0]=toupper(apptitle[0]);
		}

		mkdir( bundlePath.c_str(),0755 );
		remove( binaryPath.c_str() );

		// TODO: a bit lazy...
		if( system( ("cp "+home+"/cfg/bbexe.icns "+bundlePath+"/"+appid+".icns").c_str() ) ){
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

		plist << "</dict>\n";
		plist << "</plist>\n";

		plist.close();

		binaryPath=bundlePath+"/"+appid;
	}
#endif

	// just the name?
	args.push_back( exeFile.c_str() );

	args.push_back("--error-limit=0");

	args.push_back("--lto-O0");

#ifdef BB_MACOS
	// TODO: use xcrun --show-sdk-path
	args.push_back("-syslibroot");
	args.push_back("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk");

	// args.push_back("-lobjc");
	args.push_back("-lc");
	args.push_back("-lc++");

	args.push_back("-lSystem");
	// args.push_back("-lSystem_asan");

	args.push_back("-arch");
	args.push_back(BB_ARCH);

	args.push_back("-platform_version");args.push_back("macos");args.push_back("12.1");args.push_back("12.3");
#endif


#ifdef BB_LINUX
	// TODO: find out the best way to get this information from the environment
	// TODO: gcc --print-file-name=libc.a
	args.push_back("-L");args.push_back("/usr/lib/x86_64-linux-gnu");
	args.push_back("-L");args.push_back("/usr/lib/gcc/x86_64-linux-gnu/10");

	args.push_back("-lc");
	args.push_back("--entry");args.push_back("main");
	args.push_back("-lstdc++");
	args.push_back("-lgcc");
	args.push_back("-lgcc_eh");

	// args.push_back("--static");
#endif

// TODO: fix this hardcoding
#ifdef BB_ASAN
	#define CLANG_LIBS "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/13.1.6/lib/darwin"

	args.push_back("-rpath");args.push_back(CLANG_LIBS);
	args.push_back(CLANG_LIBS "/libclang_rt.asan_osx_dynamic.dylib");
	args.push_back(CLANG_LIBS "/libclang_rt.osx.a");
#endif


	args.push_back("-o");
	args.push_back( binaryPath.c_str() );

	args.push_back( mainObj.c_str() );

	args.push_back( "-L" );
	args.push_back( lib_dir.c_str() );

	args.push_back( "-lbb.stub" );
	args.push_back( strdup( ("-lruntime."+rt+".static").c_str() ) );

	ifstream rti( toolchain+"/runtime."+rt+".i" );
	if (!rti.is_open()) {
		cerr<<"Cannot find interface file for runtime."<<rt<<endl;
		abort();
	}

	string line;
	vector<string> modules;
	while( getline( rti,line ) ) {
		if( line.find("DEPS:")!=0 ) continue;

		size_t s=5, e=line.find(";");
		while( 1 ) {
			string frag=line.substr( s,e-s );
			while( frag[0] == ' ' ) frag=frag.substr( 1 );

			if( frag.length() == 0 ) break;

			modules.push_back( frag );

			if( e==string::npos ) {
				break;
			}

			s=e+1;
			e=line.find(";",s);
		}
	}

	for( std::string mod:modules ){
		string arg( "-lbb." + mod );
		args.push_back( strdup(arg.c_str()) );

		string line;
		ifstream iface( toolchain+"/cfg/"+mod+".i" );
		if (!iface.is_open()) {
			cerr<<"Cannot find interface file for "<<mod<<endl;
			abort();
		}

		while( getline( iface,line ) ) {
			if( line.find("LIBS:")==0 ) {
				size_t s=5, e=line.find(";");
				while( 1 ) {
					string frag=line.substr( s,e-s );
					while( frag[0] == ' ' ) frag=frag.substr( 1 );

					if( frag.length() == 0 ) break;

					string arg;
					if( frag.find("-framework")==0 ) {
						args.push_back("-framework");
						arg = frag.substr( 11 );
					} else {
						arg = "-l" + frag;
					}
					args.push_back( strdup( arg.c_str() ) );

					if( e==string::npos ) {
						break;
					}

					s=e+1;
					e=line.find(";",s);
				}
			}
		}
		iface.close();
	}

	if( !lld::FORMAT::link( args,llvm::outs(),llvm::errs(),true,false ) ) {
		exit( 1 );
	};
}
