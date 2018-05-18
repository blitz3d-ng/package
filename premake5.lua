require 'src/emcc'

-- premake5.lua
workspace "blitz3d"
  configurations { "debug", "release", "test" }
  platforms { "win32", "win64", "mingw32", "macos", "linux", "emscripten" }

  location "build"

  characterset "MBCS"
  exceptionhandling "SEH"

  flags "StaticRuntime"

  defines {
    "PRO", -- "MEMDEBUG",
    "FREEIMAGE_LIB", "OPJ_STATIC",
    "_CRT_SECURE_NO_WARNINGS",
    "DIRECTSOUND_VERSION=0x700",
    "ODE_LIB",
    "GLEW_STATIC"
  }

  -- includedirs {
  --   "freeimage317/Source",
  --   "src/runtime"
  -- }

  filter "kind:StaticLib"
    defines "_LIB"
    targetdir "_release/lib/%{cfg.platform}"

  filter "configurations:test"
    optimize "Off"
    buildoptions "-fprofile-arcs -ftest-coverage -O0"
    linkoptions "-fprofile-arcs -ftest-coverage"

  filter "configurations:debug or test"
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
    defines { "__cdecl=__attribute__((__cdecl__))" }
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
    defines { "_set_se_translator=set_se_translator" }
    linkoptions { "-static" }

  filter { "platforms:mingw32", "kind:SharedLib" }
    targetprefix ""
    targetextension ".dll"
    linkoptions "-shared"

  filter { "platforms:mingw32", "kind:StaticLib" }
    targetprefix "lib"
    targetextension ".a"

  filter { "platforms:mingw32", "kind:WindowedApp or ConsoleApp" }
    targetextension ".exe"

  filter "platforms:macos"
    toolset "clang"
    system "macosx"
    defines "BB_PLATFORM=\"macos\""

    buildoptions "-mmacosx-version-min=10.9"

  filter { "platforms:linux" }
    system "linux"

  filter { "platforms:macos or linux or mingw32" }
    buildoptions { "-Wno-error" }

  filter { "platforms:macos or linux or mingw32 or emscripten", "language:C++" }
    buildoptions { "-std=c++11" }

  filter { "platforms:macos or linux", "language:C++" }
    buildoptions { "-Wno-c++11-narrowing" }

  filter { "platforms:emscripten" }
    toolset "emcc"
    gccprefix ""
    defines "BB_PLATFORM=\"emscripten\""

  filter { "platforms:emscripten", "configurations:debug", "configurations:test" }
    buildoptions "-s DEMANGLE_SUPPORT=1"

  filter { "platforms:emscripten", "kind:StaticLib" }
    targetprefix "lib"
    targetextension ".a"

  filter { "platforms:emscripten", "kind:ConsoleApp" }
    targetprefix ""
    targetextension ".js"

  filter { "platforms:emscripten", "kind:WindowedApp" }
    targetprefix ""
    targetextension ".html"

require './build/src/init'

require './bblaunch/premake5'
require './blitzide/premake5'
require './debugger/premake5'
require './linker/premake5'

if not os.getenv("CI") then require 'src/blitzide2/premake5' end
require './compiler/premake5'
require './src/debugger.console/premake5'

require './src/deps/assimp/premake5'
require './src/deps/crossguid/premake5'
require './src/deps/enet/premake5'
require './src/deps/freeimage317/premake5'
require './src/deps/freetype2/premake5'
require './src/deps/glew/premake5'
require './src/deps/glfw3/premake5'
require './src/deps/ode/premake5'
require './src/deps/ogg/premake5'
require './src/deps/vorbis/premake5'

project "stdutil"
  kind "StaticLib"
  language "C++"

  files { "stdutil/stdutil.h", "stdutil/stdutil.cpp" }
