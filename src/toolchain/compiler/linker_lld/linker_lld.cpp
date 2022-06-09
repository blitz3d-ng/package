#include "linker_lld.h"
#include <lld/Common/Driver.h>
#include <iostream>
#include <vector>

Linker_LLD::Linker_LLD( const std::string &home, const std::string &rt ):home(home),rt(rt){
}

void Linker_LLD::createExe( const std::string &mainObj, const std::string &exeFile ){
	std::string toolchain=home+"/toolchains/" BB_PLATFORM;
	std::string lib_dir=toolchain+"/lib/" BB_ENV;

	std::vector<const char *> args;

	// just the name?
	args.push_back( exeFile.c_str() );

	args.push_back("--error-limit=0");

	args.push_back("--lto-O0");

	// TODO: use xcrun --show-sdk-path
	args.push_back("-syslibroot");
	args.push_back("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk");

	args.push_back("-o");
	args.push_back( exeFile.c_str() );

	args.push_back( mainObj.c_str() );

	// TODO: should load data from the runtime JSON or *.i files
	args.push_back( "-L" );
	args.push_back( lib_dir.c_str() );

	args.push_back("-lbb.stub");
	args.push_back("-lruntime.opengl.static");
	args.push_back("-lbb.stdio");
	args.push_back("-lbb.hook");
	args.push_back("-lbb.enet");
	args.push_back("-lbb.filesystem.posix");
	args.push_back("-lbb.graphics");
	args.push_back("-lbb.audio");
	args.push_back("-lbb.stream");
	args.push_back("-lbb.math");
	args.push_back("-lbb.runtime");
	args.push_back("-lbb.blitz2d");
	args.push_back("-lbb.blitz3d");
	args.push_back("-lbb.filesystem");
	args.push_back("-lbb.blitz2d.gl");
	args.push_back("-lbb.system");
	args.push_back("-lbb.pixmap");
	args.push_back("-lbb.runtime.glfw3");
	args.push_back("-lbb.blitz");
	args.push_back("-lbb.system.macos");
	args.push_back("-lbb.bank");
	args.push_back("-lbb.blitz3d.gl");
	args.push_back("-lbb.input");
	args.push_back("-lbb.string");
	args.push_back("-lbb.timer");
	args.push_back("-lbb.timer.noop");
	args.push_back("-lbb.sockets");
	args.push_back("-lbb.event");
	args.push_back("-lbb.audio.openal");
	args.push_back("-lbb.audio.openal");
	args.push_back("-lbb.event");
	args.push_back("-lbb.string");
	args.push_back("-lbb.input");
	args.push_back("-lbb.blitz3d.gl");
	args.push_back("-lbb.bank");
	args.push_back("-lbb.system.macos");
	args.push_back("-lbb.blitz");
	args.push_back("-lbb.runtime.glfw3");
	args.push_back("-lbb.pixmap");
	args.push_back("-lbb.system");
	args.push_back("-lbb.blitz2d.gl");
	args.push_back("-lbb.filesystem");
	args.push_back("-lbb.blitz3d");
	args.push_back("-lbb.blitz2d");
	args.push_back("-lbb.runtime");
	args.push_back("-lbb.math");
	args.push_back("-lbb.stream");
	args.push_back("-lbb.audio");
	args.push_back("-lbb.graphics.gl");
	args.push_back("-lbb.filesystem.posix");
	args.push_back("-lbb.enet");
	args.push_back("-lbb.hook");
	args.push_back("-lbb.stdio");
	args.push_back("-lenet");
	args.push_back("-lcrossguid");
	args.push_back("-lvorbis");
	args.push_back("-logg");
	args.push_back("-lm");
	args.push_back("-lfreetype2");
	args.push_back("-lassimp");
	args.push_back("-lglew");
	args.push_back("-lfreeimage");
	args.push_back("-ljpeg");
	args.push_back("-lopenexr");
	args.push_back("-lopenjpeg");
	args.push_back("-lpng");
	args.push_back("-lraw_");
	args.push_back("-ltiff4");
	args.push_back("-lwebp");
	args.push_back("-lzlibstatic");
	args.push_back("-lglfw3");
	args.push_back("-lstdutil");

	args.push_back("-framework");
	args.push_back("OpenGL");
	args.push_back("-framework");
	args.push_back("Cocoa");
	args.push_back("-framework");
	args.push_back("OpenAL");
	args.push_back("-framework");
	args.push_back("IOKit");
	args.push_back("-framework");
	args.push_back("CoreFoundation");
	args.push_back("-framework");
	args.push_back("CoreVideo");

	args.push_back("-lobjc");

#ifdef BB_DEBUG
	// std::string clang_dir="-L"+toolchain+"/llvm/lib/clang/10.0.0/lib/darwin";
	// args.push_back( clang_dir.c_str() );
	// args.push_back("-lclang_rt.asan_osx_dynamic");
#endif

	args.push_back("-lc");
	args.push_back("-lc++");
	// args.push_back("-static");

	// macos
	args.push_back("-lSystem");
	args.push_back("-arch");args.push_back("arm64");
	args.push_back("-platform_version");args.push_back("macos");args.push_back("12.1");args.push_back("12.3");

	lld::macho::link(args, llvm::outs(), llvm::errs(), true, false);
}
