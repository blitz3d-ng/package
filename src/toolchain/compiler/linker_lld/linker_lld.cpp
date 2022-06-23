#include "linker_lld.h"
#include <lld/Common/Driver.h>
#include <iostream>
#include <fstream>
#include <vector>

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

Linker_LLD::Linker_LLD( const std::string &home, const Environ *env ):home(home),env(env){
}

void Linker_LLD::createExe( const std::string &mainObj, const std::string &exeFile ){
	std::string toolchain=home+"/toolchains/" BB_TRIPLE;
	std::string lib_dir=toolchain+"/lib";

	std::vector<const char *> args;

	// just the name?
	args.push_back( exeFile.c_str() );

	args.push_back("--error-limit=0");

	// args.push_back("--lto-O0");

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
	args.push_back("-L");args.push_back("/usr/lib/llvm-11/lib/clang/11.0.1/lib/linux");

	args.push_back("-lc");
	args.push_back("-lstdc++");
	args.push_back("-lgcc");
	args.push_back("-lgcc_eh");

	// args.push_back("-lclang_rt.builtins-x86_64");

	// args.push_back("--static");
	// args.push_back("/usr/lib/x86_64-linux-gnu/crt1.o");
#endif

// TODO: fix this hardcoding
#ifdef BB_ASAN
	#define CLANG_LIBS "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/13.1.6/lib/darwin"

	args.push_back("-rpath");args.push_back(CLANG_LIBS);
	args.push_back(CLANG_LIBS "/libclang_rt.asan_osx_dynamic.dylib");
	args.push_back(CLANG_LIBS "/libclang_rt.osx.a");
#endif


	args.push_back("-o");
	args.push_back( exeFile.c_str() );

	args.push_back( mainObj.c_str() );

	args.push_back( "-L" );
	args.push_back( lib_dir.c_str() );

	args.push_back( "-lbb.stub" );
	args.push_back( strdup( ("-lruntime."+env->rt+".static").c_str() ) );

	ifstream rt( toolchain+"/runtime."+env->rt+".i" );
	if (!rt.is_open()) {
		cerr<<"Cannot find interface file for runtime."<<env->rt<<endl;
		abort();
	}

	string line;
	vector<string> modules;
	while( getline( rt,line ) ) {
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

	// args.push_back("-static");

	if( !lld::FORMAT::link(args, llvm::outs(), llvm::errs(), true, false) ) {
		exit( 1 );
	};
}
