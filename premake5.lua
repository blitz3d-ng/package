-- premake5.lua
workspace "blitz3d"
  configurations { "debug", "release" }
  platforms { "win32", "win64", "mingw32", "macos", "linux" }

  location "build"

  characterset "MBCS"
  exceptionhandling "SEH"

  flags "StaticRuntime"

  defines {
    "PRO", -- "MEMDEBUG",
    "FREEIMAGE_LIB", "OPJ_STATIC",
    "_CRT_SECURE_NO_WARNINGS",
    "DIRECTSOUND_VERSION=0x700",
    "DIRECTINPUT_VERSION=0x800",
    "GLEW_STATIC"
  }

  includedirs {
    "freeimage317/Source",
    "src/runtime"
  }

  filter "kind:StaticLib"
    defines "_LIB"

  filter "configurations:debug"
    symbols "on"

    defines { "DEBUG" }

  filter "configurations:release"
    optimize "Full"
    defines { "NDEBUG" }

  filter "platforms:win32"
    architecture "x86"
    libdirs "common/x86"
    includedirs "common/include"
    defines "BB_PLATFORM=\"windows\""

    defines { "WIN32", "TARGETSUFFIX=" }

  filter "platforms:win64"
    architecture "x64"
    libdirs "common/x64"
    includedirs "common/include"

    targetsuffix "64"

    defines { "WIN32", "WIN64", "TARGETSUFFIX=\"64\"" }
    defines "BB_PLATFORM=\"windows\""

  filter "platforms:win32 or win64"
    buildoptions {
      "/Gy" -- function level linking: true
    }

    disablewarnings { "4018","4244","4996" }

  filter { "platforms:linux" }
    defines { "_cdecl=__attribute__((__cdecl__))", "_fastcall=__fastcall", "_stdcall=__stdcall" }
    defines "BB_PLATFORM=\"linux\""

  filter { "platforms:win32 or win64", "kind:WindowedApp or ConsoleApp" }
    targetextension ".exe"

  filter "platforms:mingw32"
    libdirs "common/x86"
    includedirs "common/include"

    system "windows"

    toolset "gcc"
    gccprefix "i686-w64-mingw32-"

    defines "BB_PLATFORM=\"windows\""
    defines { "WIN32", "_WIN32", "__MINGW64_TOOLCHAIN__", "TARGETSUFFIX=", "PTW32_STATIC_LIB" }
    defines { "_declspec=__declspec", "_set_se_translator=set_se_translator" }
    linkoptions { "-static" }

  filter { "platforms:mingw32", "kind:SharedLib" }
    targetprefix ""
    targetextension ".dll"
    linkoptions "-shared"

  filter { "platforms:mingw32", "kind:WindowedApp or ConsoleApp" }
    targetextension ".exe"

  filter "platforms:macos"
    toolset "clang"
    system "macosx"
    defines "BB_PLATFORM=\"macos\""

  filter { "platforms:macos", "kind:StaticLib" }
    targetdir "_release/lib/%{cfg.platform}"

  filter { "platforms:linux" }
    system "linux"

  filter { "platforms:macos or linux or mingw32", "language:C++" }
    buildoptions { "-std=c++11" }

  filter { "platforms:macos or linux", "language:C++" }
    buildoptions { "-Wno-c++11-narrowing" }

require './src/runtime/premake/init'

require './bblaunch/premake5'
require './blitzide/premake5'
require './debugger/premake5'
require './linker/premake5'

if not os.getenv("CI") then require 'src/blitzide2/premake5' end
require './compiler/premake5'
require './src/debugger.console/premake5'
require './src/freeimage.premake5'
-- require './src/premake5.assimp'

project "stub"
  kind "StaticLib"
  language "C++"

  files {
    "src/runtime/bb/stub/stub.h"
  }

  filter "platforms:win32 or win64 or mingw32"
    files "src/runtime/bb/stub/stub.windows.cpp"

  filter "platforms:macos"
    files "src/runtime/bb/stub/stub.macos.cpp"

  filter "platforms:linux"
    files "src/runtime/bb/stub/stub.linux.cpp"

project "stdutil"
  kind "StaticLib"
  language "C++"

  files { "stdutil/stdutil.h", "stdutil/stdutil.cpp" }
