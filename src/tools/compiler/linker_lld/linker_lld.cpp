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
#define UNAME
#endif

#ifdef BB_MINGW
#define FORMAT mingw
#define UNAME
#endif

#ifdef BB_MACOS
#define FORMAT macho
#define UNAME "darwin"
#endif

#ifdef BB_LINUX
#define FORMAT elf
#define UNAME "linux"
#else
LLD_HAS_DRIVER(elf) // android/nx
#endif

LLD_HAS_DRIVER(FORMAT) // native

Linker_LLD::Linker_LLD( const std::string &home ):home(home){
}

void Linker_LLD::createExe( bool debug,const std::string &rt,const Target &target,const std::string &mainObj,const BundleInfo &bundle,const std::string &exeFile ){
	// string tmpdir=string( tmpnam(0) );
	std::string tmpdir="tmp/apk";

	std::string toolchain=home+"/bin/"+target.triple;
	std::string libdir=toolchain+"/lib";

	bool android=(target.type=="android"||target.type=="ovr");
	std::string androidsdk,ndkroot,sysroot,ndktriple,ndkarch;
	if( android ){
		char *androidhome=getenv("ANDROID_HOME");
		// TODO: verify environment at the start...
		if( androidhome==0 ){
			std::cerr<<"ANDROID_HOME is not set"<<std::endl;
			exit( 1 );
		}

		androidsdk=std::string( androidhome );
		ndkroot=androidsdk+"/ndk-bundle/toolchains/llvm/prebuilt/" UNAME "-x86_64";
		sysroot=ndkroot+"/sysroot";

		if( target.arch=="arm64-v8a" ){
			ndktriple="aarch64-linux-android";
			ndkarch="aarch64";
		}else if( target.arch=="armeabi-v7a" ){
			ndktriple="arm-linux-androideabi";
			ndkarch="arm";
		}else if( target.arch=="x86_64" ){
			ndktriple="x86_64-linux-android";
			ndkarch="x86_64";
		}else if( target.arch=="x86" ){
			ndktriple="i686-linux-android";
			ndkarch="x86";
		}
	}

	bool windows=false,nux=false,nx=false;
	if( target.type=="windows" ){
		windows=true;
	}else if( target.type=="linux" ){
		nux=true;
	}else if( target.type=="nx" ){
		nx=true;
	}

	std::string devkitpro;
	if( nx ){
		devkitpro="/opt/devkitpro";
	}

	// TODO: sort out all the lazy strdup business below...
	std::vector<std::string> args,libs,systemlibs;

	std::string binaryPath=exeFile;

	bool apk=false,app=false,ios=false;

	if( bundle.enabled||android ){
		if( android ){
			apk=true;
		}else if( target.type=="ios"||target.type=="ios-sim" ){
#ifdef BB_MACOS
			ios=true;
			app=true;
#endif
	}else if( target.host ){
#ifdef BB_MACOS
			app=true;
#endif
		}
	}

#ifdef BB_MACOS
	if( app ){
		binaryPath=binaryPath.substr( 0,binaryPath.size()-4 ); // remove .app
		std::string appid=basename( (char*)binaryPath.c_str() );
		binaryPath=exeFile+"/"+appid;

		system( ("mkdir -p "+exeFile).c_str() );
		remove( binaryPath.c_str() );
	}
#endif
	if( apk ){
		remove( exeFile.c_str() );
		system( ("rm -rf "+tmpdir).c_str() );

		// arm64-v8a armeabi-v7a x86_64 x86
		system( ("mkdir -p "+tmpdir+"/lib/arm64-v8a").c_str() );
		binaryPath=tmpdir+"/lib/arm64-v8a/libmain.so";
	}else if( nx ){
		remove( exeFile.c_str() );

		std::string dir=dirname( (char*)binaryPath.c_str() );
		std::string base=basename( (char*)binaryPath.c_str() );
		base=base.substr( 0,base.size()-4 );
		binaryPath=dir+"/"+base+".elf";
	}

	// strip unused symbols...
	if( target.host ){
#ifdef BB_MACOS
		// strip unused code
		args.push_back( "-dead_strip" );
		args.push_back( "-dead_strip_dylibs" );
		args.push_back( "--deduplicate-strings" );

		// strip debug symbols
		if( !debug ){
		  args.push_back( "-S" );args.push_back( "-x" );
		}
#endif
#ifdef BB_LINUX
		args.push_back( "--gc-sections" );
#endif
	}else if( ios ){
		args.push_back( "-dead_strip" );
	}else if( android ){
		args.push_back( "--gc-sections" );
	}else if( nx ){
		args.push_back( "--gc-sections" );
	}

	if( target.type=="ovr" ){
		args.push_back( "-u" );args.push_back( "ANativeActivity_onCreate" );
	}

	if( !windows && !nx ){
		args.push_back("--error-limit=0");
		// args.push_back("--lto-O0");
	}

	if( target.host ){
#ifdef BB_WINDOWS
		args.push_back( "/errorlimit:0" );

		// args.push_back( "/subsystem:windows" );

		args.push_back( "/nodefaultlib" );
		args.push_back( "oldnames.lib" );
		std::string winsdk="C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0";
		std::string msvc="C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Tools\\MSVC\\14.32.31326";
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
	}else if( android ){
		if( ndkarch=="aarch64" ){
			args.push_back("-m");args.push_back("aarch64linux");
		}

		args.push_back( "-shared" );
		args.push_back("-u");args.push_back("SDL_main");

		args.push_back("-z");args.push_back("noexecstack");
		args.push_back("-EL");
		if( ndkarch=="aarch64" ){
			args.push_back("--fix-cortex-a53-843419");
		}
		args.push_back("--warn-shared-textrel");
		args.push_back("-z");args.push_back("now");
		args.push_back("-z");args.push_back("relro");
		args.push_back("-z");
		args.push_back("max-page-size=4096");
		args.push_back("--hash-style=both");
		args.push_back("--enable-new-dtags");
		args.push_back("--eh-frame-hdr");

		args.push_back( sysroot+"/usr/lib/"+ndktriple+"/"+target.version+"/crtbegin_so.o");
		args.push_back("-L");args.push_back( ndkroot+"/lib64/clang/11.0.5/lib/linux/"+ndkarch);
		args.push_back("-L");args.push_back( ndkroot+"/lib/gcc/"+ndktriple+"/4.9.x");
		args.push_back("-L");args.push_back( ndkroot+"/"+ndktriple+"/lib64");
		args.push_back("-L");args.push_back( ndkroot+"/"+ndktriple+"/lib");
		args.push_back("-L");args.push_back( ndkroot+"/sysroot/usr/lib/"+ndktriple+"/"+target.version);
		args.push_back("-L");args.push_back( ndkroot+"/sysroot/usr/lib/"+ndktriple+"");
		args.push_back("-L");args.push_back( ndkroot+"/sysroot/usr/lib");
	}else if( nx ){
		args.push_back( "-maarch64elf" );
		args.push_back( "-L"+devkitpro+"/portlibs/switch/lib" );

		// TODO: read from /opt/devkitpro/libnx/switch.specs
		args.push_back( "-X" );
		args.push_back( "-EL" );
		args.push_back( "-T" );args.push_back( devkitpro+"/libnx/switch.ld" );

		args.push_back("-pie");
		args.push_back("--no-dynamic-linker");
		args.push_back( "-z" );args.push_back( "text" );
		args.push_back( "-z" );args.push_back( "now" );

		args.push_back("--build-id=sha1");
		args.push_back("--nx-module-name");
	}

#ifdef BB_MACOS
	if( target.host||target.type=="ios"||target.type=="ios-sim" ){
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
		// libs.push_back( "System_asan" );

		args.push_back("-arch");
		args.push_back( target.arch );

		args.push_back( "-platform_version" );args.push_back( platform_id );
		args.push_back( platform_version_min );args.push_back( platform_version_max );
	}
#endif

#if defined(BB_MACOS) && defined(BB_ASAN)
	if( target.host ){
		if( FILE *fp=popen("clang -v 2>&1 >/dev/null", "r") ){
			std::string clang_version,clang_dir;

			const char *dir_key="InstalledDir: ";
			const char *version_key="clang version ";
			const char *toolchain_ext=".xctoolchain";

			char path[512];
			while( fgets( path,sizeof(path),fp ) ){
				if( strncmp( path,dir_key,strlen( dir_key ) )==0 ){
					const char *start=path+strlen( dir_key );
					const char *end=strstr( start,toolchain_ext )+strlen( toolchain_ext );
					clang_dir.assign( start,end-start );
				}else if( const char *start=strstr( path,version_key ) ){
					start+=strlen( version_key );
					const char *end=start;
					while( *end && *end!=' ' ) ++end;
					clang_version.assign( start,end-start );
				}
			}
			pclose( fp );

			std::string clang_libs=clang_dir+"/usr/lib/clang/"+clang_version+"/lib/darwin";

			args.push_back( "-rpath" );args.push_back( clang_libs );
			args.push_back( clang_libs+"/libclang_rt.asan_osx_dynamic.dylib" );
			args.push_back( clang_libs+"/libclang_rt.profile_osx.a" );
			args.push_back( clang_libs+"/libclang_rt.osx.a" );
		}else{
			// TODO: figure out what exactly to do here...
		}
	}
#endif

#ifdef BB_WINDOWS
	args.push_back( "/lldignoreenv" );

	std::string machine="/machine:X64";
	args.push_back( machine );

	std::string outArg="/out:"+binaryPath;
	args.push_back( outArg );
#else
	args.push_back("-o");args.push_back( binaryPath );
#endif

#ifdef BB_WINDOWS
	std::string libPath="/libpath:"+libdir;
	args.push_back( libPath );
#else
	args.push_back( "-L"+libdir );
#endif

	libs.push_back( "runtime."+rt+".static" );

	const Target::Runtime &rti=target.runtimes.at( rt );
	for( std::string mod:rti.modules ){
		const Target::Module &m=target.modules.at( mod );
		libs.push_back( "bb."+mod );
		for( std::string lib:m.libs ) libs.push_back( lib );
		for( std::string lib:m.system_libs ) systemlibs.push_back( lib );
	}

	if( nux||nx ){
		args.push_back("--start-group");
	}

	std::string mainPath=std::string(tmpnam(0))+".o";
	std::ofstream mainFile( mainPath,std::ios_base::binary );
	mainFile.write( mainObj.c_str(),mainObj.size() );
	mainFile.flush();
	args.push_back( mainPath );

	//*------------------------

	for( auto lib:libs ){
		std::string arg;
#ifdef BB_WINDOWS
		arg=lib+std::string(".lib");
#else
		arg="-l"+std::string(lib);
#endif
		args.push_back( arg );
	}

	for( auto lib:systemlibs ){
#ifdef BB_WINDOWS
		args.push_back( lib+".lib" );
#else
		std::string fw="-framework";
		if( lib.find( fw )==0 ) {
			args.push_back( fw );
			args.push_back( lib.substr( fw.size()+1 ) );
		} else {
			args.push_back( "-l"+lib );
		}
#endif
	}

	if( nux||nx ){
		args.push_back("--end-group");
	}

	if( target.host ){
#ifdef BB_LINUX
		args.push_back("-dynamic-linker");args.push_back("/lib64/ld-linux-x86-64.so.2");

		// TODO: find out the best way to get this information from the environment
		// TODO: gcc --print-file-name=libc.a
		args.push_back("-L");args.push_back( LIBGCC_DIR );
		args.push_back("-L");args.push_back( "/lib" );
		args.push_back("-L");args.push_back( LIBARCH_DIR );
		args.push_back("-L");args.push_back( "/usr/lib" );
#endif
	}else if( android ){
		args.push_back("-dynamic-linker");args.push_back("/system/bin/linker64");
	}

	if( target.host ){
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
	}else if( android ){
		args.push_back("-l");args.push_back("c++_static");
		args.push_back("-l");args.push_back("c++abi");
		args.push_back("-l");args.push_back("gcc");
		args.push_back("-l");args.push_back("c");
		args.push_back("-l");args.push_back("gcc");
		args.push_back( ndkroot+"/sysroot/usr/lib/"+ndktriple+"/"+target.version+"/crtend_so.o" );
	}else if( nx ){
		std::string gcc=devkitpro+"/devkitA64/lib/gcc/aarch64-none-elf/13.2.0/pic";
		args.push_back( "-L"+devkitpro+"/devkitA64/aarch64-none-elf/lib/pic" );
		args.push_back( "-L"+devkitpro+"/libnx/lib" );
		args.push_back( "-L"+gcc );

		args.push_back( gcc+"/crti.o");
		args.push_back( gcc+"/crtbegin.o");

		args.push_back("-lstdc++");
		args.push_back("--start-group");
		args.push_back("-lgcc");
		args.push_back("-lg");
		args.push_back("-lc");
		args.push_back("-lsysbase");
		args.push_back("-lpthread");
		args.push_back("--end-group");

		args.push_back( gcc+"/crtend.o");
		args.push_back( gcc+"/crtn.o");
	}

	bool success=false;
	if( nx ){
		// TODO: seems like it's possible to use the LLD elf linker, but there are
		// some more config that needs to be discovered
		std::string cmdline="";
		for( auto arg:args ) cmdline+=arg+" ";

		success=system( ("/opt/devkitpro/devkitA64/bin/aarch64-none-elf-ld "+cmdline).c_str() )!=-1;
	}else{
		std::vector<char *> _args;_args.reserve( args.size()+1 );

		// just the name?
		_args.push_back( strdup( "linker" ) );
		for( auto arg:args ) _args.push_back( strdup( arg.c_str() ) );

		auto link=android?lld::elf::link:lld::FORMAT::link;
		success=link( _args,llvm::outs(),llvm::errs(),false,false );
		for( auto s:_args ) free( (char*)s );
	}

	remove( mainPath.c_str() );

	if( !success ){
		std::cerr<<"failed to link"<<std::endl;
		exit( 1 );
	}

#ifdef BB_MACOS
	if( app ){
		createApp( exeFile,home,bundle,target );
	}
#endif
	if( apk ){
		createApk( exeFile,tmpdir,home,toolchain,bundle,target,rt,androidsdk );
	}else if ( nx ){
		createNRO( exeFile,home,bundle,target,binaryPath );
	}
}
