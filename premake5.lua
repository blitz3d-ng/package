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
    "DIRECTINPUT_VERSION=0x800"
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

    defines { "WIN32", "TARGETSUFFIX=" }

  filter "platforms:win64"
    architecture "x64"
    libdirs "common/x64"
    includedirs "common/include"

    targetsuffix "64"

    defines { "WIN32", "WIN64", "TARGETSUFFIX=\"64\"" }

  filter "platforms:win32 or win64"
    buildoptions {
      "/Gy" -- function level linking: true
    }

    disablewarnings { "4018","4244","4996" }

  filter { "platforms:macos or mingw32" }
    defines { "_cdecl=__cdecl", "_fastcall=__fastcall", "_stdcall=__stdcall" }

  filter { "platforms:linux" }
    defines { "_cdecl=__attribute__((__cdecl__))", "_fastcall=__fastcall", "_stdcall=__stdcall" }

  filter { "platforms:win32 or win64", "kind:WindowedApp or ConsoleApp" }
    targetextension ".exe"

  filter "platforms:mingw32"
    libdirs "common/x86"
    includedirs "common/include"

    system "windows"

    toolset "gcc"
    gccprefix "i686-w64-mingw32-"

    defines { "WIN32", "_WIN32", "__MINGW64_TOOLCHAIN__", "TARGETSUFFIX=", "PTW32_STATIC_LIB" }
    defines { "_declspec=__declspec", "_set_se_translator=set_se_translator" }
    linkoptions { "-static" }

  filter { "platforms:mingw32", "files:**.cpp" }
    buildoptions { "-std=c++11" }

  filter { "platforms:mingw32", "kind:SharedLib" }
    targetprefix ""
    targetextension ".dll"
    linkoptions "-shared"

  filter { "platforms:mingw32", "kind:WindowedApp or ConsoleApp" }
    targetextension ".exe"

  filter "platforms:macos"
    toolset "clang"
    system "macosx"

  filter { "platforms:macos", "kind:StaticLib" }
    targetdir "_release/lib/%{cfg.platform}"

  filter { "platforms:linux" }
    system "linux"

  filter { "platforms:linux", "files:**.cpp" }
    buildoptions "-std=c++11"

require './src/runtime/premake/init'

require './bblaunch/premake5'
require './blitzide/premake5'
require './debugger/premake5'
require './linker/premake5'

if not os.getenv("CI") then require 'src/blitzide2/premake5' end
require './compiler/premake5'
require './src/debugger.console/premake5'
require './src/freeimage.premake5'

project "stub"
  kind "StaticLib"
  language "C++"

  files "src/runtime/bb/stub/stub.h"

  filter "platforms:win32 or win64 or mingw32"
    files "src/runtime/bb/stub/stub.windows.cpp"

  filter "platforms:macos"
    files "src/runtime/bb/stub/stub.macos.cpp"

  filter "platforms:linux"
    files "src/runtime/bb/stub/stub.linux.cpp"

project "frame"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files {
    "src/runtime/bb/frame/frame.cpp",
    "src/runtime/bb/frame/frame.h"
  }

project "gxruntime"
  kind "StaticLib"
  language "C++"

  removeplatforms { "win64", "macos", "linux" }

  files { "gxruntime/gxgraphics.cpp", "gxruntime/gxmovie.cpp", "gxruntime/gxruntime.cpp", "gxruntime/gxgraphics.h", "gxruntime/gxmovie.h", "gxruntime/gxruntime.h", "gxruntime/std.h" }

project "multiplayer"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files {
    "src/runtime/bb/multiplayer/multiplay.cpp", "src/runtime/bb/multiplayer/multiplay.h",
    "src/runtime/bb/multiplayer/multiplay_setup.cpp", "src/runtime/bb/multiplayer/multiplay_setup.h",
    "src/runtime/bb/multiplayer/multiplay_setup.rc"
  }

project "runtime"
  kind "StaticLib"
  language "C++"

  files {
    "src/runtime/bb/runtime/runtime.h",
    "src/runtime/bb/runtime/runtime.cpp"
  }

project "blitz"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files {
    "src/runtime/bb/blitz/app.cpp", "src/runtime/bb/blitz/app.h",
    "src/runtime/bb/blitz/basic.cpp", "src/runtime/bb/blitz/basic.h",
    "src/runtime/bb/blitz/debug.cpp", "src/runtime/bb/blitz/debug.h",
    "src/runtime/bb/blitz/env.cpp", "src/runtime/bb/blitz/env.h",
    "src/runtime/bb/blitz/ex.cpp", "src/runtime/bb/blitz/ex.h",
    "src/runtime/bb/blitz/module.h"
  }

project "audio.fmod"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  includedirs "common/include"

  files {
    "src/runtime/bb/audio.fmod/sound.cpp", "src/runtime/bb/audio.fmod/sound.h",
    "src/runtime/bb/audio.fmod/driver.cpp", "src/runtime/bb/audio.fmod/driver.h"
  }

  links "audio"

project "system.windows"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files {
    "src/runtime/bb/system.windows/system.windows.cpp",
    "src/runtime/bb/system.windows/system.windows.h"
  }

  links "system"

project "blitz3d"
  kind "StaticLib"
  language "C++"

  files {
    "src/runtime/bb/blitz3d/animation.cpp", "src/runtime/bb/blitz3d/animator.cpp", "src/runtime/bb/blitz3d/blitz3d.h", "src/runtime/bb/blitz3d/blitz3d.cpp", "src/runtime/bb/blitz3d/brush.cpp", "src/runtime/bb/blitz3d/cachedtexture.cpp", "src/runtime/bb/blitz3d/camera.cpp", "src/runtime/bb/blitz3d/collision.cpp", "src/runtime/bb/blitz3d/entity.cpp", "src/runtime/bb/blitz3d/frustum.cpp", "src/runtime/bb/blitz3d/geom.cpp", "src/runtime/bb/blitz3d/light.cpp", "src/runtime/bb/blitz3d/listener.cpp", "src/runtime/bb/blitz3d/loader_3ds.cpp", "src/runtime/bb/blitz3d/loader_b3d.cpp", "src/runtime/bb/blitz3d/md2model.cpp", "src/runtime/bb/blitz3d/md2norms.cpp", "src/runtime/bb/blitz3d/md2rep.cpp", "src/runtime/bb/blitz3d/meshcollider.cpp", "src/runtime/bb/blitz3d/meshloader.cpp", "src/runtime/bb/blitz3d/meshmodel.cpp", "src/runtime/bb/blitz3d/meshutil.cpp", "src/runtime/bb/blitz3d/mirror.cpp", "src/runtime/bb/blitz3d/model.cpp", "src/runtime/bb/blitz3d/object.cpp", "src/runtime/bb/blitz3d/pivot.cpp", "src/runtime/bb/blitz3d/planemodel.cpp", "src/runtime/bb/blitz3d/q3bspmodel.cpp", "src/runtime/bb/blitz3d/q3bsprep.cpp", "src/runtime/bb/blitz3d/scene.cpp", "src/runtime/bb/blitz3d/sprite.cpp", "src/runtime/bb/blitz3d/std.cpp", "src/runtime/bb/blitz3d/surface.cpp", "src/runtime/bb/blitz3d/terrain.cpp", "src/runtime/bb/blitz3d/terrainrep.cpp", "src/runtime/bb/blitz3d/texture.cpp", "src/runtime/bb/blitz3d/world.cpp", "src/runtime/bb/blitz3d/animation.h", "src/runtime/bb/blitz3d/animator.h", "src/runtime/bb/blitz3d/blitz3d.h", "src/runtime/bb/blitz3d/brush.h", "src/runtime/bb/blitz3d/cachedtexture.h", "src/runtime/bb/blitz3d/camera.h", "src/runtime/bb/blitz3d/collision.h", "src/runtime/bb/blitz3d/entity.h", "src/runtime/bb/blitz3d/frustum.h", "src/runtime/bb/blitz3d/geom.h", "src/runtime/bb/blitz3d/light.h", "src/runtime/bb/blitz3d/listener.h", "src/runtime/bb/blitz3d/loader_3ds.h", "src/runtime/bb/blitz3d/loader_b3d.h", "src/runtime/bb/blitz3d/md2model.h", "src/runtime/bb/blitz3d/md2norms.h", "src/runtime/bb/blitz3d/md2rep.h", "src/runtime/bb/blitz3d/meshcollider.h", "src/runtime/bb/blitz3d/meshloader.h", "src/runtime/bb/blitz3d/meshmodel.h", "src/runtime/bb/blitz3d/meshutil.h", "src/runtime/bb/blitz3d/mirror.h", "src/runtime/bb/blitz3d/model.h", "src/runtime/bb/blitz3d/object.h", "src/runtime/bb/blitz3d/pivot.h", "src/runtime/bb/blitz3d/planemodel.h", "src/runtime/bb/blitz3d/q3bspmodel.h", "src/runtime/bb/blitz3d/q3bsprep.h", "src/runtime/bb/blitz3d/rendercontext.h", "src/runtime/bb/blitz3d/scene.h", "src/runtime/bb/blitz3d/sprite.h", "src/runtime/bb/blitz3d/std.h", "src/runtime/bb/blitz3d/surface.h", "src/runtime/bb/blitz3d/terrain.h", "src/runtime/bb/blitz3d/terrainrep.h", "src/runtime/bb/blitz3d/texture.h", "src/runtime/bb/blitz3d/world.h"
  }

filter "platforms:win32 or win64 or mingw32"
  files { "src/runtime/bb/blitz3d/loader_x.cpp", "src/runtime/bb/blitz3d/loader_x.h" }

project "sockets"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files {
    "src/runtime/bb/sockets/sockets.cpp", "src/runtime/bb/sockets/sockets.h"
  }

project "graphics"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files {
    "src/runtime/bb/graphics/graphics.h", "src/runtime/bb/graphics/graphics.cpp",
    "src/runtime/bb/graphics/canvas.h", "src/runtime/bb/graphics/canvas.cpp"
  }

project "stdutil"
  kind "StaticLib"
  language "C++"

  files { "stdutil/stdutil.h", "stdutil/stdutil.cpp" }
