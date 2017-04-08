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

  filter { "platforms:win32 or win64", "kind:WindowedApp or ConsoleApp" }
    targetextension ".exe"

  filter "platforms:mingw32"
    libdirs "common/x86"
    includedirs "common/include"

    toolset "gcc"
    gccprefix "i686-w64-mingw32-"

    defines { "WIN32", "_WIN32", "__MINGW64_TOOLCHAIN__", "TARGETSUFFIX=", "PTW32_STATIC_LIB" }

    defines { "_cdecl=__cdecl", "_fastcall=__fastcall", "_stdcall=__stdcall", "_declspec=__declspec", "_set_se_translator=set_se_translator" }
    linkoptions { "-static" }

  filter { "platforms:mingw32", "language:C++"}
    buildoptions { "-std=c++11" }

  filter { "platforms:mingw32", "kind:SharedLib" }
    targetprefix ""
    targetextension ".dll"
    -- linkoptions { "-static-libstdc++", "-static-libgcc" }

  filter { "platforms:mingw32", "kind:WindowedApp or ConsoleApp" }
    targetextension ".exe"
    -- linkoptions { "-static-libstdc++", "-static-libgcc" }

  filter "platforms:macos"
    toolset "clang"

  filter { "platforms:linux", "language:C++"}
    buildoptions "-std=c++11"

project "blitzide"
  kind "WindowedApp"
  language "C++"

  removeplatforms { "macos", "linux", "mingw32" }

  targetdir "_release/bin"
  targetname "ide"
  targetextension ".exe"

  -- characterset "Unicode"

  warnings "Off"
  disablewarnings { "4100","4189" }

  defines {
    "_WIN32_WINNT=_WIN32_WINNT_WINXP",
    "NO_WARN_MBCS_MFC_DEPRECATION"
  }

  flags { "WinMain", "MFC" }

  files {
    "blitzide/about.cpp", "blitzide/about.h",
    "blitzide/blitzide.cpp", "blitzide/blitzide.h",
    "blitzide/editor.cpp", "blitzide/editor.h",
    "blitzide/funclist.cpp", "blitzide/funclist.h",
    "blitzide/htmlhelp.cpp", "blitzide/htmlhelp.h",
    "blitzide/libs.cpp", "blitzide/libs.h",
    "blitzide/mainframe.cpp", "blitzide/mainframe.h",
    "blitzide/prefs.cpp", "blitzide/prefs.h",
    "blitzide/stdafx.cpp", "blitzide/stdafx.h",
    "blitzide/tabber.cpp", "blitzide/tabber.h",
    "blitzide/dpi.cpp", "blitzide/dpi.h",
    "blitzide/blitzide.rc"
  }

if not os.getenv("CI") then
  project "blitzide2"
    kind "WindowedApp"
    language "C++"

    if not os.getenv("WXWIDGETS") then
      removeplatforms { "win32", "win64", "mingw32" }
    end

    characterset "Unicode"

    targetdir "_release/bin"
    targetname "ide2"

    files {
      "blitzide2/HtmlHelp.h", "blitzide2/HtmlHelp.cpp",
      "blitzide2/FileView.h", "blitzide2/FileView.cpp",
      "blitzide2/main.cpp"
    }

    filter "platforms:win32 or win64"
      flags "WinMain"
      includedirs {
        "C:\\wxWidgets-3.0.2\\include\\msvc",
        "C:\\wxWidgets-3.0.2\\include"
      }

    filter "platforms:win32"
      libdirs "C:\\wxWidgets-3.0.2\\lib\\vc_lib"

    filter "platforms:win64"
      libdirs "C:\\wxWidgets-3.0.2\\lib\\vc_x64_lib"

    filter "platforms:macos or linux"
      buildoptions {
        "-std=c++11",
        "`wx-config --cflags std webview stc`"
      }
      linkoptions "`wx-config --libs std webview stc`"
end

project "debugger"
  kind "SharedLib"
  language "C++"

  removeplatforms { "win64", "mingw32", "macos", "linux" }

  targetdir "_release/bin"
  targetname "debugger"
  targetextension ".dll"

  buildoptions "/w"

  defines {
    "_WIN32_WINNT=_WIN32_WINNT_WINXP",
    "_USRDLL"
  }

  links "stdutil"

  flags "MFC"

  files {
    "debugger/debugger.rc",
    "debugger/debuggerapp.cpp", "debugger/debuggerapp.h",
    "debugger/debugtree.cpp", "debugger/debugtree.h",
    "debugger/mainframe.cpp", "debugger/mainframe.h",
    "debugger/prefs.cpp", "debugger/prefs.h",
    "debugger/sourcefile.cpp", "debugger/sourcefile.h",
    "debugger/stdafx.cpp", "debugger/stdafx.h",
    "debugger/tabber.cpp", "debugger/tabber.h",
    "debugger/dpi.cpp", "debugger/dpi.h",
    "debugger/debugger.h"
  }

project "bblaunch"
  kind "WindowedApp"
  language "C++"

  removeplatforms { "macos", "linux" }

  files { "bblaunch/bblaunch.cpp" }

  filter "platforms:win32 or win64 or mingw32"
    files { "bblaunch/checkdx.cpp", "bblaunch/checkdx.h", "bblaunch/checkie.cpp", "bblaunch/checkie.h", "bblaunch/bblaunch.rc", "re" }
    flags "WinMain"

  targetdir "_release"
  targetname "Blitz3D"
  targetextension ".exe"

  characterset "Unicode"
  defines "UNICODE" -- needed for mingw32

  links { "dxguid", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "odbc32", "odbccp32" }

runtimes = { 'default', 'dplay' }

for i,rt in ipairs(runtimes) do
  project("runtime." .. rt)
    kind "SharedLib"
    language "C++"

    removeplatforms { "win64", "macos", "linux" }

    targetdir "_release/bin"
    targetprefix ""

    local STUB_PATH="src/runtime/" .. rt .. ".stub.cpp"

    files {
      "bbruntime_dll/bbruntime_dll.h",
      "bbruntime_dll/bbruntime_dll.cpp",
      "bbruntime_dll/bbruntime_dll.rc",
      "bbruntime_dll/resource.h",
      "bbruntime_dll/dpi.manifest",
      STUB_PATH
    }

    links "stub"

    local config = require("src/runtime/" .. rt)

    local stub = io.open( STUB_PATH,"w" )

    stub:write("\n#include <bb/stub/stub.h>\n\n")

    for j,mod in ipairs(config.modules) do
      stub:write("BBMODULE_DECL( " .. mod .. " );\n")
    end

    local function write_create_calls(modules, i)
      local function write(s)
        for j=1,i do stub:write("\t") end
        stub:write(s)
      end

      write("if ( " .. modules[i] .. "_create() ){\n")
      if i < #modules then
        write_create_calls( modules,i+1 )
        write("\t" .. modules[i] .. "_destroy();\n");
      else
        write("\treturn true;\n")
      end
      write("}else sue( \"" .. modules[i] .. "_create failed\" );\n")
    end

    stub:write("\nbool bbruntime_create(){\n")
    write_create_calls(config.modules, 1)
    stub:write("\treturn false;\n")
    stub:write("}\n")

    stub:write("\nvoid bbruntime_link( void (*link)( const char *sym,void *pc ) ){\n")
    for j,mod in ipairs(config.modules) do
      stub:write("\t" .. mod .. "_link( link );\n")
    end
    stub:write("}\n")

    stub:write("\nbool bbruntime_destroy(){\n")
    for j=#config.modules,1,-1 do
      stub:write("\t" .. config.modules[j] .. "_destroy();\n")
    end
    stub:write("\treturn true;\n")
    stub:write("}\n")
    stub:close()

    filter {}
    links(config.modules)
end

project "stub"
  kind "StaticLib"
  language "C++"

  files { "src/runtime/bb/stub/stub.cpp", "src/runtime/bb/stub/stub.h" }

project "gxruntime"
  kind "StaticLib"
  language "C++"

  removeplatforms { "win64", "macos", "linux" }

  files { "gxruntime/ddutil.cpp", "gxruntime/gxcanvas.cpp", "gxruntime/gxgraphics.cpp", "gxruntime/gxlight.cpp", "gxruntime/gxmesh.cpp", "gxruntime/gxmovie.cpp", "gxruntime/gxruntime.cpp", "gxruntime/gxscene.cpp", "gxruntime/std.cpp", "gxruntime/asmcoder.h", "gxruntime/ddutil.h", "gxruntime/gxcanvas.h", "gxruntime/gxgraphics.h", "gxruntime/gxlight.h", "gxruntime/gxmesh.h", "gxruntime/gxmovie.h", "gxruntime/gxruntime.h", "gxruntime/gxscene.h", "gxruntime/std.h" }

project "multiplayer"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files {
    "src/runtime/bb/multiplayer/multiplay.cpp", "src/runtime/bb/multiplayer/multiplay.h",
    "src/runtime/bb/multiplayer/multiplay_setup.cpp", "src/runtime/bb/multiplayer/multiplay_setup.h",
    "src/runtime/bb/multiplayer/multiplay_setup.rc"
  }

project "userlibs"
  kind "StaticLib"
  language "C++"

  files { "src/runtime/bb/userlibs/userlibs.cpp", "src/runtime/bb/userlibs/userlibs.h" }

project "blitz"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files {
    "src/runtime/bb/blitz/basic.cpp", "src/runtime/bb/blitz/basic.h",
    "src/runtime/bb/blitz/debug.cpp", "src/runtime/bb/blitz/debug.h",
    "src/runtime/bb/blitz/env.cpp", "src/runtime/bb/blitz/env.h",
    "src/runtime/bb/blitz/ex.h",
    "src/runtime/bb/blitz/module.h"
  }

project "audio"
  kind "StaticLib"
  language "C++"

  files {
    "src/runtime/bb/audio/channel.cpp", "src/runtime/bb/audio/channel.h",
    "src/runtime/bb/audio/sound.h",
    "src/runtime/bb/audio/driver.cpp", "src/runtime/bb/audio/driver.h",
    "src/runtime/bb/audio/module.cpp", "src/runtime/bb/audio/module.h"
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

project "bank"
  kind "StaticLib"
  language "C++"

  files {
    "src/runtime/bb/bank/bank.cpp", "src/runtime/bb/bank/bank.h"
  }

project "math"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files { "src/runtime/bb/math/math.cpp", "src/runtime/bb/math/math.h"  }

project "system"
  kind "StaticLib"
  language "C++"

  files { "src/runtime/bb/system/driver.cpp", "src/runtime/bb/system/driver.h" }

project "system.windows"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files {
    "src/runtime/bb/system.windows/driver.cpp", "src/runtime/bb/system.windows/driver.h",
    "src/runtime/bb/system.windows/system.windows.cpp", "src/runtime/bb/system.windows/system.windows.h"
  }

  links "system"

project "filesystem"
  kind "StaticLib"
  language "C++"

  files {
    "src/runtime/bb/filesystem/filesystem.cpp", "src/runtime/bb/filesystem/filesystem.h",
    "src/runtime/bb/filesystem/driver.cpp", "src/runtime/bb/filesystem/driver.h"
  }

project "filesystem.windows"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files {
    "src/runtime/bb/filesystem.windows/driver.cpp", "src/runtime/bb/filesystem.windows/driver.h",
    "src/runtime/bb/filesystem.windows/dir.cpp", "src/runtime/bb/filesystem.windows/dir.h"
  }

  links "filesystem"

project "input"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos" }

  files {
    "src/runtime/bb/input/input.cpp", "src/runtime/bb/input/input.h",
    "src/runtime/bb/input/driver.cpp", "src/runtime/bb/input/driver.h",
    "src/runtime/bb/input/device.cpp", "src/runtime/bb/input/device.h"
  }

project "input.directinput8"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files { "src/runtime/bb/input.directinput8/driver.cpp", "src/runtime/bb/input.directinput8/driver.h" }

project "blitz3d"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos" }

  files {
    "src/runtime/bb/blitz3d/animation.cpp", "src/runtime/bb/blitz3d/animator.cpp", "src/runtime/bb/blitz3d/blitz3d.h", "src/runtime/bb/blitz3d/blitz3d.cpp", "src/runtime/bb/blitz3d/brush.cpp", "src/runtime/bb/blitz3d/cachedtexture.cpp", "src/runtime/bb/blitz3d/camera.cpp", "src/runtime/bb/blitz3d/collision.cpp", "src/runtime/bb/blitz3d/entity.cpp", "src/runtime/bb/blitz3d/frustum.cpp", "src/runtime/bb/blitz3d/geom.cpp", "src/runtime/bb/blitz3d/light.cpp", "src/runtime/bb/blitz3d/listener.cpp", "src/runtime/bb/blitz3d/loader_3ds.cpp", "src/runtime/bb/blitz3d/loader_b3d.cpp", "src/runtime/bb/blitz3d/md2model.cpp", "src/runtime/bb/blitz3d/md2norms.cpp", "src/runtime/bb/blitz3d/md2rep.cpp", "src/runtime/bb/blitz3d/meshcollider.cpp", "src/runtime/bb/blitz3d/meshloader.cpp", "src/runtime/bb/blitz3d/meshmodel.cpp", "src/runtime/bb/blitz3d/meshutil.cpp", "src/runtime/bb/blitz3d/mirror.cpp", "src/runtime/bb/blitz3d/model.cpp", "src/runtime/bb/blitz3d/object.cpp", "src/runtime/bb/blitz3d/pivot.cpp", "src/runtime/bb/blitz3d/planemodel.cpp", "src/runtime/bb/blitz3d/q3bspmodel.cpp", "src/runtime/bb/blitz3d/q3bsprep.cpp", "src/runtime/bb/blitz3d/sprite.cpp", "src/runtime/bb/blitz3d/std.cpp", "src/runtime/bb/blitz3d/surface.cpp", "src/runtime/bb/blitz3d/terrain.cpp", "src/runtime/bb/blitz3d/terrainrep.cpp", "src/runtime/bb/blitz3d/texture.cpp", "src/runtime/bb/blitz3d/world.cpp", "src/runtime/bb/blitz3d/animation.h", "src/runtime/bb/blitz3d/animator.h", "src/runtime/bb/blitz3d/blitz3d.h", "src/runtime/bb/blitz3d/brush.h", "src/runtime/bb/blitz3d/cachedtexture.h", "src/runtime/bb/blitz3d/camera.h", "src/runtime/bb/blitz3d/collision.h", "src/runtime/bb/blitz3d/entity.h", "src/runtime/bb/blitz3d/frustum.h", "src/runtime/bb/blitz3d/geom.h", "src/runtime/bb/blitz3d/light.h", "src/runtime/bb/blitz3d/listener.h", "src/runtime/bb/blitz3d/loader_3ds.h", "src/runtime/bb/blitz3d/loader_b3d.h", "src/runtime/bb/blitz3d/md2model.h", "src/runtime/bb/blitz3d/md2norms.h", "src/runtime/bb/blitz3d/md2rep.h", "src/runtime/bb/blitz3d/meshcollider.h", "src/runtime/bb/blitz3d/meshloader.h", "src/runtime/bb/blitz3d/meshmodel.h", "src/runtime/bb/blitz3d/meshutil.h", "src/runtime/bb/blitz3d/mirror.h", "src/runtime/bb/blitz3d/model.h", "src/runtime/bb/blitz3d/object.h", "src/runtime/bb/blitz3d/pivot.h", "src/runtime/bb/blitz3d/planemodel.h", "src/runtime/bb/blitz3d/q3bspmodel.h", "src/runtime/bb/blitz3d/q3bsprep.h", "src/runtime/bb/blitz3d/rendercontext.h", "src/runtime/bb/blitz3d/sprite.h", "src/runtime/bb/blitz3d/std.h", "src/runtime/bb/blitz3d/surface.h", "src/runtime/bb/blitz3d/terrain.h", "src/runtime/bb/blitz3d/terrainrep.h", "src/runtime/bb/blitz3d/texture.h", "src/runtime/bb/blitz3d/world.h"
  }

filter "platforms:win32 or win64 or mingw32"
  files { "src/runtime/bb/blitz3d/loader_x.cpp", "src/runtime/bb/blitz3d/loader_x.h" }

project "stream"
  kind "StaticLib"
  language "C++"

  files {
    "src/runtime/bb/stream/stream.cpp", "src/runtime/bb/stream/stream.h"
  }

project "string"
  kind "StaticLib"
  language "C++"

  files {
    "src/runtime/bb/string/string.cpp", "src/runtime/bb/string/string.h"
  }

project "sockets"
  kind "StaticLib"
  language "C++"

  files {
    "src/runtime/bb/sockets/sockets.cpp", "src/runtime/bb/sockets/sockets.h"
  }

project "timer"
  kind "StaticLib"
  language "C++"

  files { "src/runtime/bb/timer/timer.h", "src/runtime/bb/timer/timer.cpp" }

project "timer.windows"
  kind "StaticLib"
  language "C++"

  removeplatforms { "macos", "linux" }

  files { "src/runtime/bb/timer.windows/timer.windows.h", "src/runtime/bb/timer.windows/timer.windows.cpp" }

project "blitz2d"
  kind "StaticLib"
  language "C++"

  files { "src/runtime/bb/blitz2d/font.h", "src/runtime/bb/blitz2d/font.cpp", "src/runtime/bb/blitz2d/graphics.h" }

project "graphics"
  kind "StaticLib"
  language "C++"

  files {
    "src/runtime/bb/graphics/graphics.h", "src/runtime/bb/graphics/graphics.cpp",
    "src/runtime/bb/graphics/canvas.h", "src/runtime/bb/graphics/canvas.cpp"
  }

project "compiler"
  kind "ConsoleApp"
  language "C++"

  removeplatforms { "win64", "macos", "linux" }

  targetdir "_release/bin"
  targetname "blitzcc"

  files {
    "compiler/main.cpp", "compiler/libs.cpp",
    "compiler/declnode.cpp", "compiler/declnode.h", "compiler/exprnode.cpp", "compiler/exprnode.h", "compiler/node.cpp", "compiler/node.h", "compiler/nodes.h", "compiler/prognode.cpp", "compiler/prognode.h", "compiler/stmtnode.cpp", "compiler/stmtnode.h", "compiler/varnode.cpp", "compiler/varnode.h", "compiler/decl.cpp", "compiler/decl.h", "compiler/environ.cpp", "compiler/environ.h", "compiler/label.h", "compiler/type.cpp", "compiler/type.h", "compiler/parser.cpp", "compiler/parser.h", "compiler/toker.cpp", "compiler/toker.h",
    "compiler/codegen_x86/codegen_x86.cpp", "compiler/codegen_x86/codegen_x86.h", "compiler/codegen_x86/tile.cpp", "compiler/codegen_x86/tile.h",
    "compiler/codegen.h",
    "compiler/assem_x86/asm_insts.cpp", "compiler/assem_x86/assem_x86.cpp", "compiler/assem_x86/assem_x86.h", "compiler/assem_x86/insts.h", "compiler/assem_x86/operand.cpp", "compiler/assem_x86/operand.h",
    "compiler/assem.h", "compiler/ex.h", "compiler/std.cpp", "compiler/std.h"
  }

  links { "stdutil" }

  filter "platforms:mingw32" -- FIXME: move ScaleBitmap out of stdutil so this isn't needed.
    links "gdi32"

  filter "platforms:win32"
    files { "compiler/resource.h", "compiler/blitz.rc", "bbruntime_dll/dpi.manifest" }

project "linker"
  kind "SharedLib"
  language "C++"

  removeplatforms { "win64", "macos", "linux" }

  targetdir "_release/bin"

  linkoptions { "-shared" }

  files { "linker/main.cpp", "linker/linker.h", "linker/linker.cpp", "linker/image_util.h", "linker/image_util.cpp" }
  links { "stdutil" }

  filter "platforms:mingw32" -- FIXME: move ScaleBitmap out of stdutil so this isn't needed.
    links "gdi32"

project "stdutil"
  kind "StaticLib"
  language "C++"

  files { "stdutil/stdutil.h", "stdutil/stdutil.cpp" }

project "freeimage"
  kind "StaticLib"
  language "C++"

  defines "LIBRAW_NODLL"

  includedirs {
    "freeimage317/Source/LibJXR/jxrgluelib",
    "freeimage317/Source/LibJXR/image/sys",
    "freeimage317/Source/OpenEXR/Half",
    "freeimage317/Source/OpenEXR/Iex",
    "freeimage317/Source/OpenEXR/lexMath",
    "freeimage317/Source/OpenEXR/IlmImf",
    "freeimage317/Source/OpenEXR/IlmThread",
    "freeimage317/Source/OpenEXR/Imath",
    "freeimage317/Source/OpenEXR",
    "freeimage317/Source/Zlib"
  }

  files {
    "freeimage317/Source/FreeImage/BitmapAccess.cpp", "freeimage317/Source/FreeImage/ColorLookup.cpp", "freeimage317/Source/FreeImage/ConversionRGBA16.cpp", "freeimage317/Source/FreeImage/ConversionRGBAF.cpp", "freeimage317/Source/FreeImage/FreeImage.cpp", "freeimage317/Source/FreeImage/FreeImageC.c", "freeimage317/Source/FreeImage/FreeImageIO.cpp", "freeimage317/Source/FreeImage/GetType.cpp", "freeimage317/Source/FreeImage/LFPQuantizer.cpp", "freeimage317/Source/FreeImage/MemoryIO.cpp", "freeimage317/Source/FreeImage/PixelAccess.cpp", "freeimage317/Source/FreeImage/J2KHelper.cpp", "freeimage317/Source/FreeImage/MNGHelper.cpp", "freeimage317/Source/FreeImage/Plugin.cpp", "freeimage317/Source/FreeImage/PluginBMP.cpp", "freeimage317/Source/FreeImage/PluginCUT.cpp", "freeimage317/Source/FreeImage/PluginDDS.cpp", "freeimage317/Source/FreeImage/PluginEXR.cpp", "freeimage317/Source/FreeImage/PluginG3.cpp", "freeimage317/Source/FreeImage/PluginGIF.cpp", "freeimage317/Source/FreeImage/PluginHDR.cpp", "freeimage317/Source/FreeImage/PluginICO.cpp", "freeimage317/Source/FreeImage/PluginIFF.cpp", "freeimage317/Source/FreeImage/PluginJ2K.cpp", "freeimage317/Source/FreeImage/PluginJNG.cpp", "freeimage317/Source/FreeImage/PluginJP2.cpp", "freeimage317/Source/FreeImage/PluginJPEG.cpp", "freeimage317/Source/FreeImage/PluginKOALA.cpp", "freeimage317/Source/FreeImage/PluginMNG.cpp", "freeimage317/Source/FreeImage/PluginPCD.cpp", "freeimage317/Source/FreeImage/PluginPCX.cpp", "freeimage317/Source/FreeImage/PluginPFM.cpp", "freeimage317/Source/FreeImage/PluginPICT.cpp", "freeimage317/Source/FreeImage/PluginPNG.cpp", "freeimage317/Source/FreeImage/PluginPNM.cpp", "freeimage317/Source/FreeImage/PluginPSD.cpp", "freeimage317/Source/FreeImage/PluginRAS.cpp", "freeimage317/Source/FreeImage/PluginRAW.cpp", "freeimage317/Source/FreeImage/PluginSGI.cpp", "freeimage317/Source/FreeImage/PluginTARGA.cpp", "freeimage317/Source/FreeImage/PluginTIFF.cpp", "freeimage317/Source/FreeImage/PluginWBMP.cpp", "freeimage317/Source/FreeImage/PluginWebP.cpp", "freeimage317/Source/FreeImage/PluginXBM.cpp", "freeimage317/Source/FreeImage/PluginXPM.cpp", "freeimage317/Source/FreeImage/PSDParser.cpp", "freeimage317/Source/FreeImage/TIFFLogLuv.cpp", "freeimage317/Source/FreeImage/Conversion.cpp", "freeimage317/Source/FreeImage/Conversion16_555.cpp", "freeimage317/Source/FreeImage/Conversion16_565.cpp", "freeimage317/Source/FreeImage/Conversion24.cpp", "freeimage317/Source/FreeImage/Conversion32.cpp", "freeimage317/Source/FreeImage/Conversion4.cpp", "freeimage317/Source/FreeImage/Conversion8.cpp", "freeimage317/Source/FreeImage/ConversionFloat.cpp", "freeimage317/Source/FreeImage/ConversionRGB16.cpp", "freeimage317/Source/FreeImage/ConversionRGBF.cpp", "freeimage317/Source/FreeImage/ConversionType.cpp", "freeimage317/Source/FreeImage/ConversionUINT16.cpp", "freeimage317/Source/FreeImage/Halftoning.cpp", "freeimage317/Source/FreeImage/tmoColorConvert.cpp", "freeimage317/Source/FreeImage/tmoDrago03.cpp", "freeimage317/Source/FreeImage/tmoFattal02.cpp", "freeimage317/Source/FreeImage/tmoReinhard05.cpp", "freeimage317/Source/FreeImage/ToneMapping.cpp", "freeimage317/Source/FreeImage/NNQuantizer.cpp", "freeimage317/Source/FreeImage/WuQuantizer.cpp", "freeimage317/Source/DeprecationManager/Deprecated.cpp", "freeimage317/Source/DeprecationManager/DeprecationMgr.cpp", "freeimage317/Source/FreeImage/CacheFile.cpp", "freeimage317/Source/FreeImage/MultiPage.cpp", "freeimage317/Source/FreeImage/ZLibInterface.cpp", "freeimage317/Source/Metadata/Exif.cpp", "freeimage317/Source/Metadata/FIRational.cpp", "freeimage317/Source/Metadata/FreeImageTag.cpp", "freeimage317/Source/Metadata/IPTC.cpp", "freeimage317/Source/Metadata/TagConversion.cpp", "freeimage317/Source/Metadata/TagLib.cpp", "freeimage317/Source/Metadata/XTIFF.cpp", "freeimage317/Source/FreeImageToolkit/Background.cpp", "freeimage317/Source/FreeImageToolkit/BSplineRotate.cpp", "freeimage317/Source/FreeImageToolkit/Channels.cpp", "freeimage317/Source/FreeImageToolkit/ClassicRotate.cpp", "freeimage317/Source/FreeImageToolkit/Colors.cpp", "freeimage317/Source/FreeImageToolkit/CopyPaste.cpp", "freeimage317/Source/FreeImageToolkit/Display.cpp", "freeimage317/Source/FreeImageToolkit/Flip.cpp", "freeimage317/Source/FreeImageToolkit/JPEGTransform.cpp", "freeimage317/Source/FreeImageToolkit/MultigridPoissonSolver.cpp", "freeimage317/Source/FreeImageToolkit/Rescale.cpp", "freeimage317/Source/FreeImageToolkit/Resize.cpp", "freeimage317/Source/CacheFile.h", "freeimage317/Source/DeprecationManager/DeprecationMgr.h", "freeimage317/Source/MapIntrospector.h", "freeimage317/Source/Metadata/FIRational.h", "freeimage317/Source/FreeImage.h", "freeimage317/Source/FreeImageIO.h", "freeimage317/Source/Metadata/FreeImageTag.h", "freeimage317/Source/FreeImage/J2KHelper.h", "freeimage317/Source/Plugin.h", "freeimage317/Source/FreeImage/PSDParser.h", "freeimage317/Source/Quantizers.h", "freeimage317/Source/ToneMapping.h", "freeimage317/Source/Utilities.h", "freeimage317/Source/FreeImageToolkit/Resize.h"
  }

  filter "platforms:win32 or win64"
    files {
      "freeimage317/Source/FreeImage/PluginJXR.cpp",
      "freeimage317/FreeImage.rc"
    }

  filter { "platforms:mingw32" }
    files "common/freeimage_jxr_stub.cpp"

project "jpeg"
  kind "StaticLib"
  language "C"

  files {
    "freeimage317/Source/LibJPEG/jaricom.c", "freeimage317/Source/LibJPEG/jcapimin.c", "freeimage317/Source/LibJPEG/jcapistd.c", "freeimage317/Source/LibJPEG/jcarith.c", "freeimage317/Source/LibJPEG/jccoefct.c", "freeimage317/Source/LibJPEG/jccolor.c", "freeimage317/Source/LibJPEG/jcdctmgr.c", "freeimage317/Source/LibJPEG/jchuff.c", "freeimage317/Source/LibJPEG/jcinit.c", "freeimage317/Source/LibJPEG/jcmainct.c", "freeimage317/Source/LibJPEG/jcmarker.c", "freeimage317/Source/LibJPEG/jcmaster.c", "freeimage317/Source/LibJPEG/jcomapi.c", "freeimage317/Source/LibJPEG/jcparam.c", "freeimage317/Source/LibJPEG/jcprepct.c", "freeimage317/Source/LibJPEG/jcsample.c", "freeimage317/Source/LibJPEG/jctrans.c", "freeimage317/Source/LibJPEG/jdapimin.c", "freeimage317/Source/LibJPEG/jdapistd.c", "freeimage317/Source/LibJPEG/jdarith.c", "freeimage317/Source/LibJPEG/jdatadst.c", "freeimage317/Source/LibJPEG/jdatasrc.c", "freeimage317/Source/LibJPEG/jdcoefct.c", "freeimage317/Source/LibJPEG/jdcolor.c", "freeimage317/Source/LibJPEG/jddctmgr.c", "freeimage317/Source/LibJPEG/jdhuff.c", "freeimage317/Source/LibJPEG/jdinput.c", "freeimage317/Source/LibJPEG/jdmainct.c", "freeimage317/Source/LibJPEG/jdmarker.c", "freeimage317/Source/LibJPEG/jdmaster.c", "freeimage317/Source/LibJPEG/jdmerge.c", "freeimage317/Source/LibJPEG/jdpostct.c", "freeimage317/Source/LibJPEG/jdsample.c", "freeimage317/Source/LibJPEG/jdtrans.c", "freeimage317/Source/LibJPEG/jerror.c", "freeimage317/Source/LibJPEG/jfdctflt.c", "freeimage317/Source/LibJPEG/jfdctfst.c", "freeimage317/Source/LibJPEG/jfdctint.c", "freeimage317/Source/LibJPEG/jidctflt.c", "freeimage317/Source/LibJPEG/jidctfst.c", "freeimage317/Source/LibJPEG/jidctint.c", "freeimage317/Source/LibJPEG/jmemmgr.c", "freeimage317/Source/LibJPEG/jmemnobs.c", "freeimage317/Source/LibJPEG/jquant1.c", "freeimage317/Source/LibJPEG/jquant2.c", "freeimage317/Source/LibJPEG/jutils.c", "freeimage317/Source/LibJPEG/transupp.c"
  }

project "jxr"
  removeplatforms { "macos" }
  kind "StaticLib"
  language "C"

  includedirs {
    "freeimage317/Source/LibJXR/jxrgluelib",
    "freeimage317/Source/LibJXR/common/include",
    "freeimage317/Source/LibJXR/image/sys"
  }

  defines "DISABLE_PERF_MEASUREMENT"

  files {
    "freeimage317/Source/LibJXR/image/decode/decode.c", "freeimage317/Source/LibJXR/image/decode/JXRTranscode.c", "freeimage317/Source/LibJXR/image/decode/postprocess.c", "freeimage317/Source/LibJXR/image/decode/segdec.c", "freeimage317/Source/LibJXR/image/decode/strdec.c", "freeimage317/Source/LibJXR/image/decode/strdec_x86.c", "freeimage317/Source/LibJXR/image/decode/strInvTransform.c", "freeimage317/Source/LibJXR/image/decode/strPredQuantDec.c", "freeimage317/Source/LibJXR/image/encode/encode.c", "freeimage317/Source/LibJXR/image/encode/segenc.c", "freeimage317/Source/LibJXR/image/encode/strenc.c", "freeimage317/Source/LibJXR/image/encode/strenc_x86.c", "freeimage317/Source/LibJXR/image/encode/strFwdTransform.c", "freeimage317/Source/LibJXR/image/encode/strPredQuantEnc.c", "freeimage317/Source/LibJXR/image/sys/adapthuff.c", "freeimage317/Source/LibJXR/image/sys/image.c", "freeimage317/Source/LibJXR/image/sys/strcodec.c", "freeimage317/Source/LibJXR/image/sys/strPredQuant.c", "freeimage317/Source/LibJXR/image/sys/strTransform.c", "freeimage317/Source/LibJXR/jxrgluelib/JXRGlue.c", "freeimage317/Source/LibJXR/jxrgluelib/JXRGlueJxr.c", "freeimage317/Source/LibJXR/jxrgluelib/JXRGluePFC.c", "freeimage317/Source/LibJXR/jxrgluelib/JXRMeta.c", "freeimage317/Source/LibJXR/image/decode/decode.h", "freeimage317/Source/LibJXR/image/encode/encode.h", "freeimage317/Source/LibJXR/image/sys/ansi.h", "freeimage317/Source/LibJXR/image/sys/common.h", "freeimage317/Source/LibJXR/image/sys/perfTimer.h", "freeimage317/Source/LibJXR/image/sys/strcodec.h", "freeimage317/Source/LibJXR/image/sys/strTransform.h", "freeimage317/Source/LibJXR/image/sys/windowsmediaphoto.h", "freeimage317/Source/LibJXR/image/sys/xplatform_image.h", "freeimage317/Source/LibJXR/image/x86/x86.h", "freeimage317/Source/LibJXR/common/include/guiddef.h", "freeimage317/Source/LibJXR/common/include/wmsal.h", "freeimage317/Source/LibJXR/common/include/wmspecstring.h", "freeimage317/Source/LibJXR/common/include/wmspecstrings_adt.h", "freeimage317/Source/LibJXR/common/include/wmspecstrings_strict.h", "freeimage317/Source/LibJXR/common/include/wmspecstrings_undef.h", "freeimage317/Source/LibJXR/jxrgluelib/JXRGlue.h", "freeimage317/Source/LibJXR/jxrgluelib/JXRMeta.h"
  }

  filter "platforms:linux"
    defines { "__ANSI__" }

project "openexr"
  kind "StaticLib"
  language "C++"

  includedirs {
    "freeimage317/Source/OpenEXR/Half",
    "freeimage317/Source/OpenEXR/Iex",
    "freeimage317/Source/OpenEXR/IexMath",
    "freeimage317/Source/OpenEXR/IlmImf",
    "freeimage317/Source/OpenEXR/IlmThread",
    "freeimage317/Source/OpenEXR/Imath",
    "freeimage317/Source/OpenEXR",
    "freeimage317/Source/Zlib"
  }

  files {
    "freeimage317/Source/OpenEXR/IexMath/IexMathFpu.cpp", "freeimage317/Source/OpenEXR/IlmImf/b44ExpLogTable.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfAcesFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfB44Compressor.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfBoxAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfChannelList.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfChannelListAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfChromaticities.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfChromaticitiesAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfCompositeDeepScanLine.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfCompressionAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfCompressor.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfConvert.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfCRgbaFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepCompositing.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepFrameBuffer.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepImageStateAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepScanLineInputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepScanLineInputPart.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepScanLineOutputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepScanLineOutputPart.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepTiledInputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepTiledInputPart.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepTiledOutputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepTiledOutputPart.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDoubleAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfDwaCompressor.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfEnvmap.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfEnvmapAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfFastHuf.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfFloatAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfFloatVectorAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfFrameBuffer.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfFramesPerSecond.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfGenericInputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfGenericOutputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfHeader.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfHuf.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfInputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfInputPart.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfInputPartData.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfIntAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfIO.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfKeyCode.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfKeyCodeAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfLineOrderAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfLut.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfMatrixAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfMisc.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfMultiPartInputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfMultiPartOutputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfMultiView.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfOpaqueAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfOutputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfOutputPart.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfOutputPartData.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfPartType.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfPizCompressor.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfPreviewImage.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfPreviewImageAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfPxr24Compressor.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfRational.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfRationalAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfRgbaFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfRgbaYca.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfRle.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfRleCompressor.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfScanLineInputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfStandardAttributes.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfStdIO.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfStringAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfStringVectorAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfSystemSpecific.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfTestFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfThreading.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfTileDescriptionAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledInputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledInputPart.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledMisc.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledOutputFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledOutputPart.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledRgbaFile.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfTileOffsets.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfTimeCode.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfTimeCodeAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfVecAttribute.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfVersion.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfWav.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfZip.cpp", "freeimage317/Source/OpenEXR/IlmImf/ImfZipCompressor.cpp", "freeimage317/Source/OpenEXR/Imath/ImathBox.cpp", "freeimage317/Source/OpenEXR/Imath/ImathColorAlgo.cpp", "freeimage317/Source/OpenEXR/Imath/ImathFun.cpp", "freeimage317/Source/OpenEXR/Imath/ImathMatrixAlgo.cpp", "freeimage317/Source/OpenEXR/Imath/ImathRandom.cpp", "freeimage317/Source/OpenEXR/Imath/ImathShear.cpp", "freeimage317/Source/OpenEXR/Imath/ImathVec.cpp", "freeimage317/Source/OpenEXR/Iex/IexBaseExc.cpp", "freeimage317/Source/OpenEXR/Iex/IexThrowErrnoExc.cpp", "freeimage317/Source/OpenEXR/Half/half.cpp", "freeimage317/Source/OpenEXR/IlmThread/IlmThread.cpp", "freeimage317/Source/OpenEXR/IlmThread/IlmThreadMutex.cpp", "freeimage317/Source/OpenEXR/IlmThread/IlmThreadPool.cpp", "freeimage317/Source/OpenEXR/IlmThread/IlmThreadSemaphore.cpp", "freeimage317/Source/OpenEXR/IexMath/IexMathFloatExc.cpp", "freeimage317/Source/OpenEXR/IlmBaseConfig.h", "freeimage317/Source/OpenEXR/OpenEXRConfig.h", "freeimage317/Source/OpenEXR/IlmImf/b44ExpLogTable.h", "freeimage317/Source/OpenEXR/IlmImf/dwaLookups.h", "freeimage317/Source/OpenEXR/IlmImf/ImfAcesFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfArray.h", "freeimage317/Source/OpenEXR/IlmImf/ImfAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfAutoArray.h", "freeimage317/Source/OpenEXR/IlmImf/ImfB44Compressor.h", "freeimage317/Source/OpenEXR/IlmImf/ImfBoxAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfChannelList.h", "freeimage317/Source/OpenEXR/IlmImf/ImfChannelListAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfCheckedArithmetic.h", "freeimage317/Source/OpenEXR/IlmImf/ImfChromaticities.h", "freeimage317/Source/OpenEXR/IlmImf/ImfChromaticitiesAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfCompositeDeepScanLine.h", "freeimage317/Source/OpenEXR/IlmImf/ImfCompression.h", "freeimage317/Source/OpenEXR/IlmImf/ImfCompressionAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfCompressor.h", "freeimage317/Source/OpenEXR/IlmImf/ImfConvert.h", "freeimage317/Source/OpenEXR/IlmImf/ImfCRgbaFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepCompositing.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepFrameBuffer.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepImageState.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepImageStateAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepScanLineInputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepScanLineInputPart.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepScanLineOutputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepScanLineOutputPart.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepTiledInputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepTiledInputPart.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepTiledOutputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDeepTiledOutputPart.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDoubleAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDwaCompressor.h", "freeimage317/Source/OpenEXR/IlmImf/ImfDwaCompressorSimd.h", "freeimage317/Source/OpenEXR/IlmImf/ImfEnvmap.h", "freeimage317/Source/OpenEXR/IlmImf/ImfEnvmapAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfExport.h", "freeimage317/Source/OpenEXR/IlmImf/ImfFastHuf.h", "freeimage317/Source/OpenEXR/IlmImf/ImfFloatAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfFloatVectorAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfForward.h", "freeimage317/Source/OpenEXR/IlmImf/ImfFrameBuffer.h", "freeimage317/Source/OpenEXR/IlmImf/ImfFramesPerSecond.h", "freeimage317/Source/OpenEXR/IlmImf/ImfGenericInputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfGenericOutputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfHeader.h", "freeimage317/Source/OpenEXR/IlmImf/ImfHuf.h", "freeimage317/Source/OpenEXR/IlmImf/ImfInputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfInputPart.h", "freeimage317/Source/OpenEXR/IlmImf/ImfInputPartData.h", "freeimage317/Source/OpenEXR/IlmImf/ImfInputStreamMutex.h", "freeimage317/Source/OpenEXR/IlmImf/ImfInt64.h", "freeimage317/Source/OpenEXR/IlmImf/ImfIntAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfIO.h", "freeimage317/Source/OpenEXR/IlmImf/ImfKeyCode.h", "freeimage317/Source/OpenEXR/IlmImf/ImfKeyCodeAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfLineOrder.h", "freeimage317/Source/OpenEXR/IlmImf/ImfLineOrderAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfLut.h", "freeimage317/Source/OpenEXR/IlmImf/ImfMatrixAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfMisc.h", "freeimage317/Source/OpenEXR/IlmImf/ImfMultiPartInputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfMultiPartOutputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfMultiView.h", "freeimage317/Source/OpenEXR/IlmImf/ImfName.h", "freeimage317/Source/OpenEXR/IlmImf/ImfNamespace.h", "freeimage317/Source/OpenEXR/IlmImf/ImfOpaqueAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfOptimizedPixelReading.h", "freeimage317/Source/OpenEXR/IlmImf/ImfOutputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfOutputPart.h", "freeimage317/Source/OpenEXR/IlmImf/ImfOutputPartData.h", "freeimage317/Source/OpenEXR/IlmImf/ImfOutputStreamMutex.h", "freeimage317/Source/OpenEXR/IlmImf/ImfPartHelper.h", "freeimage317/Source/OpenEXR/IlmImf/ImfPartType.h", "freeimage317/Source/OpenEXR/IlmImf/ImfPixelType.h", "freeimage317/Source/OpenEXR/IlmImf/ImfPizCompressor.h", "freeimage317/Source/OpenEXR/IlmImf/ImfPreviewImage.h", "freeimage317/Source/OpenEXR/IlmImf/ImfPreviewImageAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfPxr24Compressor.h", "freeimage317/Source/OpenEXR/IlmImf/ImfRational.h", "freeimage317/Source/OpenEXR/IlmImf/ImfRationalAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfRgba.h", "freeimage317/Source/OpenEXR/IlmImf/ImfRgbaFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfRgbaYca.h", "freeimage317/Source/OpenEXR/IlmImf/ImfRle.h", "freeimage317/Source/OpenEXR/IlmImf/ImfRleCompressor.h", "freeimage317/Source/OpenEXR/IlmImf/ImfScanLineInputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfSimd.h", "freeimage317/Source/OpenEXR/IlmImf/ImfStandardAttributes.h", "freeimage317/Source/OpenEXR/IlmImf/ImfStdIO.h", "freeimage317/Source/OpenEXR/IlmImf/ImfStringAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfStringVectorAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfSystemSpecific.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTestFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfThreading.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTileDescription.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTileDescriptionAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledInputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledInputPart.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledMisc.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledOutputFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledOutputPart.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTiledRgbaFile.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTileOffsets.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTimeCode.h", "freeimage317/Source/OpenEXR/IlmImf/ImfTimeCodeAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfVecAttribute.h", "freeimage317/Source/OpenEXR/IlmImf/ImfVersion.h", "freeimage317/Source/OpenEXR/IlmImf/ImfWav.h", "freeimage317/Source/OpenEXR/IlmImf/ImfXdr.h", "freeimage317/Source/OpenEXR/IlmImf/ImfZip.h", "freeimage317/Source/OpenEXR/IlmImf/ImfZipCompressor.h", "freeimage317/Source/OpenEXR/Imath/ImathBox.h", "freeimage317/Source/OpenEXR/Imath/ImathBoxAlgo.h", "freeimage317/Source/OpenEXR/Imath/ImathColor.h", "freeimage317/Source/OpenEXR/Imath/ImathColorAlgo.h", "freeimage317/Source/OpenEXR/Imath/ImathEuler.h", "freeimage317/Source/OpenEXR/Imath/ImathExc.h", "freeimage317/Source/OpenEXR/Imath/ImathExport.h", "freeimage317/Source/OpenEXR/Imath/ImathForward.h", "freeimage317/Source/OpenEXR/Imath/ImathFrame.h", "freeimage317/Source/OpenEXR/Imath/ImathFrustum.h", "freeimage317/Source/OpenEXR/Imath/ImathFrustumTest.h", "freeimage317/Source/OpenEXR/Imath/ImathFun.h", "freeimage317/Source/OpenEXR/Imath/ImathGL.h", "freeimage317/Source/OpenEXR/Imath/ImathGLU.h", "freeimage317/Source/OpenEXR/Imath/ImathHalfLimits.h", "freeimage317/Source/OpenEXR/Imath/ImathInt64.h", "freeimage317/Source/OpenEXR/Imath/ImathInterval.h", "freeimage317/Source/OpenEXR/Imath/ImathLimits.h", "freeimage317/Source/OpenEXR/Imath/ImathLine.h", "freeimage317/Source/OpenEXR/Imath/ImathLineAlgo.h", "freeimage317/Source/OpenEXR/Imath/ImathMath.h", "freeimage317/Source/OpenEXR/Imath/ImathMatrix.h", "freeimage317/Source/OpenEXR/Imath/ImathMatrixAlgo.h", "freeimage317/Source/OpenEXR/Imath/ImathNamespace.h", "freeimage317/Source/OpenEXR/Imath/ImathPlane.h", "freeimage317/Source/OpenEXR/Imath/ImathPlatform.h", "freeimage317/Source/OpenEXR/Imath/ImathQuat.h", "freeimage317/Source/OpenEXR/Imath/ImathRandom.h", "freeimage317/Source/OpenEXR/Imath/ImathRoots.h", "freeimage317/Source/OpenEXR/Imath/ImathShear.h", "freeimage317/Source/OpenEXR/Imath/ImathSphere.h", "freeimage317/Source/OpenEXR/Imath/ImathVec.h", "freeimage317/Source/OpenEXR/Imath/ImathVecAlgo.h", "freeimage317/Source/OpenEXR/Iex/Iex.h", "freeimage317/Source/OpenEXR/Iex/IexBaseExc.h", "freeimage317/Source/OpenEXR/Iex/IexErrnoExc.h", "freeimage317/Source/OpenEXR/Iex/IexExport.h", "freeimage317/Source/OpenEXR/Iex/IexForward.h", "freeimage317/Source/OpenEXR/Iex/IexMacros.h", "freeimage317/Source/OpenEXR/Iex/IexMathExc.h", "freeimage317/Source/OpenEXR/Iex/IexNamespace.h", "freeimage317/Source/OpenEXR/Iex/IexThrowErrnoExc.h", "freeimage317/Source/OpenEXR/Half/eLut.h", "freeimage317/Source/OpenEXR/Half/half.h", "freeimage317/Source/OpenEXR/Half/halfFunction.h", "freeimage317/Source/OpenEXR/Half/halfLimits.h", "freeimage317/Source/OpenEXR/Half/toFloat.h", "freeimage317/Source/OpenEXR/IlmThread/IlmThread.h", "freeimage317/Source/OpenEXR/IlmThread/IlmThreadExport.h", "freeimage317/Source/OpenEXR/IlmThread/IlmThreadForward.h", "freeimage317/Source/OpenEXR/IlmThread/IlmThreadMutex.h", "freeimage317/Source/OpenEXR/IlmThread/IlmThreadNamespace.h", "freeimage317/Source/OpenEXR/IlmThread/IlmThreadPool.h", "freeimage317/Source/OpenEXR/IlmThread/IlmThreadSemaphore.h", "freeimage317/Source/OpenEXR/IexMath/IexMathFloatExc.h", "freeimage317/Source/OpenEXR/IexMath/IexMathIeeeExc.h"
  }

project "openjpeg"
  kind "StaticLib"
  language "C"

  files {
    "freeimage317/Source/LibOpenJPEG/bio.c", "freeimage317/Source/LibOpenJPEG/cio.c", "freeimage317/Source/LibOpenJPEG/dwt.c", "freeimage317/Source/LibOpenJPEG/event.c", "freeimage317/Source/LibOpenJPEG/function_list.c", "freeimage317/Source/LibOpenJPEG/image.c", "freeimage317/Source/LibOpenJPEG/invert.c", "freeimage317/Source/LibOpenJPEG/j2k.c", "freeimage317/Source/LibOpenJPEG/jp2.c", "freeimage317/Source/LibOpenJPEG/mct.c", "freeimage317/Source/LibOpenJPEG/mqc.c", "freeimage317/Source/LibOpenJPEG/openjpeg.c", "freeimage317/Source/LibOpenJPEG/opj_clock.c", "freeimage317/Source/LibOpenJPEG/pi.c", "freeimage317/Source/LibOpenJPEG/raw.c", "freeimage317/Source/LibOpenJPEG/t1.c", "freeimage317/Source/LibOpenJPEG/t2.c", "freeimage317/Source/LibOpenJPEG/tcd.c", "freeimage317/Source/LibOpenJPEG/tgt.c", "freeimage317/Source/LibOpenJPEG/bio.h", "freeimage317/Source/LibOpenJPEG/cio.h", "freeimage317/Source/LibOpenJPEG/dwt.h", "freeimage317/Source/LibOpenJPEG/event.h", "freeimage317/Source/LibOpenJPEG/function_list.h", "freeimage317/Source/LibOpenJPEG/image.h", "freeimage317/Source/LibOpenJPEG/indexbox_manager.h", "freeimage317/Source/LibOpenJPEG/invert.h", "freeimage317/Source/LibOpenJPEG/j2k.h", "freeimage317/Source/LibOpenJPEG/jp2.h", "freeimage317/Source/LibOpenJPEG/mct.h", "freeimage317/Source/LibOpenJPEG/mqc.h", "freeimage317/Source/LibOpenJPEG/openjpeg.h", "freeimage317/Source/LibOpenJPEG/opj_clock.h", "freeimage317/Source/LibOpenJPEG/opj_config.h", "freeimage317/Source/LibOpenJPEG/opj_config_private.h", "freeimage317/Source/LibOpenJPEG/opj_includes.h", "freeimage317/Source/LibOpenJPEG/opj_intmath.h", "freeimage317/Source/LibOpenJPEG/opj_inttypes.h", "freeimage317/Source/LibOpenJPEG/opj_malloc.h", "freeimage317/Source/LibOpenJPEG/opj_stdint.h", "freeimage317/Source/LibOpenJPEG/pi.h", "freeimage317/Source/LibOpenJPEG/raw.h", "freeimage317/Source/LibOpenJPEG/t1.h", "freeimage317/Source/LibOpenJPEG/t1_luts.h", "freeimage317/Source/LibOpenJPEG/t2.h", "freeimage317/Source/LibOpenJPEG/tcd.h", "freeimage317/Source/LibOpenJPEG/tgt.h"
  }

project "png"
  kind "StaticLib"
  language "C"

  includedirs "freeimage317/Source/Zlib"

  files {
    "freeimage317/Source/LibPNG/png.c", "freeimage317/Source/LibPNG/pngerror.c", "freeimage317/Source/LibPNG/pngget.c", "freeimage317/Source/LibPNG/pngmem.c", "freeimage317/Source/LibPNG/pngpread.c", "freeimage317/Source/LibPNG/pngread.c", "freeimage317/Source/LibPNG/pngrio.c", "freeimage317/Source/LibPNG/pngrtran.c", "freeimage317/Source/LibPNG/pngrutil.c", "freeimage317/Source/LibPNG/pngset.c", "freeimage317/Source/LibPNG/pngtrans.c", "freeimage317/Source/LibPNG/pngwio.c", "freeimage317/Source/LibPNG/pngwrite.c", "freeimage317/Source/LibPNG/pngwtran.c", "freeimage317/Source/LibPNG/pngwutil.c"
  }

project "raw"
  kind "StaticLib"
  language "C++"

  includedirs "freeimage317/Source/LibRawLite"

  defines "LIBRAW_NODLL"

  files {
    "freeimage317/Source/LibRawLite/internal/dcraw_common.cpp", "freeimage317/Source/LibRawLite/internal/dcraw_fileio.cpp", "freeimage317/Source/LibRawLite/internal/demosaic_packs.cpp", "freeimage317/Source/LibRawLite/src/libraw_c_api.cpp", "freeimage317/Source/LibRawLite/src/libraw_cxx.cpp", "freeimage317/Source/LibRawLite/src/libraw_datastream.cpp", "freeimage317/Source/LibRawLite/internal/defines.h", "freeimage317/Source/LibRawLite/internal/libraw_internal_funcs.h", "freeimage317/Source/LibRawLite/internal/var_defines.h", "freeimage317/Source/LibRawLite/libraw/libraw.h", "freeimage317/Source/LibRawLite/libraw/libraw_alloc.h", "freeimage317/Source/LibRawLite/libraw/libraw_const.h", "freeimage317/Source/LibRawLite/libraw/libraw_datastream.h", "freeimage317/Source/LibRawLite/libraw/libraw_internal.h", "freeimage317/Source/LibRawLite/libraw/libraw_types.h", "freeimage317/Source/LibRawLite/libraw/libraw_version.h"
  }

  filter "platforms:mingw32"
    buildoptions "-Wno-narrowing"

project "tiff4"
  kind "StaticLib"
  language "C"

  defines 'TIFF_SSIZE_FORMAT=""'

  files {
    "freeimage317/Source/LibTIFF4/tif_aux.c", "freeimage317/Source/LibTIFF4/tif_close.c", "freeimage317/Source/LibTIFF4/tif_codec.c", "freeimage317/Source/LibTIFF4/tif_color.c", "freeimage317/Source/LibTIFF4/tif_compress.c", "freeimage317/Source/LibTIFF4/tif_dir.c", "freeimage317/Source/LibTIFF4/tif_dirinfo.c", "freeimage317/Source/LibTIFF4/tif_dirread.c", "freeimage317/Source/LibTIFF4/tif_dirwrite.c", "freeimage317/Source/LibTIFF4/tif_dumpmode.c", "freeimage317/Source/LibTIFF4/tif_error.c", "freeimage317/Source/LibTIFF4/tif_extension.c", "freeimage317/Source/LibTIFF4/tif_fax3.c", "freeimage317/Source/LibTIFF4/tif_fax3sm.c", "freeimage317/Source/LibTIFF4/tif_flush.c", "freeimage317/Source/LibTIFF4/tif_getimage.c", "freeimage317/Source/LibTIFF4/tif_jpeg.c", "freeimage317/Source/LibTIFF4/tif_luv.c", "freeimage317/Source/LibTIFF4/tif_lzma.c", "freeimage317/Source/LibTIFF4/tif_lzw.c", "freeimage317/Source/LibTIFF4/tif_next.c", "freeimage317/Source/LibTIFF4/tif_ojpeg.c", "freeimage317/Source/LibTIFF4/tif_open.c", "freeimage317/Source/LibTIFF4/tif_packbits.c", "freeimage317/Source/LibTIFF4/tif_pixarlog.c", "freeimage317/Source/LibTIFF4/tif_predict.c", "freeimage317/Source/LibTIFF4/tif_print.c", "freeimage317/Source/LibTIFF4/tif_read.c", "freeimage317/Source/LibTIFF4/tif_strip.c", "freeimage317/Source/LibTIFF4/tif_swab.c", "freeimage317/Source/LibTIFF4/tif_thunder.c", "freeimage317/Source/LibTIFF4/tif_tile.c", "freeimage317/Source/LibTIFF4/tif_version.c", "freeimage317/Source/LibTIFF4/tif_warning.c", "freeimage317/Source/LibTIFF4/tif_write.c", "freeimage317/Source/LibTIFF4/tif_zip.c", "freeimage317/Source/LibTIFF4/t4.h", "freeimage317/Source/LibTIFF4/tif_config.h", "freeimage317/Source/LibTIFF4/tif_dir.h", "freeimage317/Source/LibTIFF4/tif_fax3.h", "freeimage317/Source/LibTIFF4/tif_predict.h", "freeimage317/Source/LibTIFF4/tiff.h", "freeimage317/Source/LibTIFF4/tiffconf.h", "freeimage317/Source/LibTIFF4/tiffio.h", "freeimage317/Source/LibTIFF4/tiffiop.h", "freeimage317/Source/LibTIFF4/tiffvers.h", "freeimage317/Source/LibTIFF4/uvcode.h"
  }

project "webp"
  kind "StaticLib"
  language "C"

  files {
    "freeimage317/Source/LibWebP/src/dec/alphai.h", "freeimage317/Source/LibWebP/src/dec/decode_vp8.h", "freeimage317/Source/LibWebP/src/dec/vp8i.h", "freeimage317/Source/LibWebP/src/dec/vp8li.h", "freeimage317/Source/LibWebP/src/dec/webpi.h", "freeimage317/Source/LibWebP/src/dsp/dsp.h", "freeimage317/Source/LibWebP/src/dsp/lossless.h", "freeimage317/Source/LibWebP/src/dsp/mips_macro.h", "freeimage317/Source/LibWebP/src/dsp/neon.h", "freeimage317/Source/LibWebP/src/dsp/yuv.h", "freeimage317/Source/LibWebP/src/dsp/yuv_tables_sse2.h", "freeimage317/Source/LibWebP/src/enc/backward_references.h", "freeimage317/Source/LibWebP/src/enc/cost.h", "freeimage317/Source/LibWebP/src/enc/histogram.h", "freeimage317/Source/LibWebP/src/enc/vp8enci.h", "freeimage317/Source/LibWebP/src/enc/vp8li.h", "freeimage317/Source/LibWebP/src/webp/decode.h", "freeimage317/Source/LibWebP/src/webp/demux.h", "freeimage317/Source/LibWebP/src/webp/encode.h", "freeimage317/Source/LibWebP/src/webp/format_constants.h", "freeimage317/Source/LibWebP/src/webp/mux.h", "freeimage317/Source/LibWebP/src/webp/mux_types.h", "freeimage317/Source/LibWebP/src/webp/types.h", "freeimage317/Source/LibWebP/src/utils/bit_reader.h", "freeimage317/Source/LibWebP/src/utils/bit_reader_inl.h", "freeimage317/Source/LibWebP/src/utils/bit_writer.h", "freeimage317/Source/LibWebP/src/utils/color_cache.h", "freeimage317/Source/LibWebP/src/utils/endian_inl.h", "freeimage317/Source/LibWebP/src/utils/filters.h", "freeimage317/Source/LibWebP/src/utils/huffman.h", "freeimage317/Source/LibWebP/src/utils/huffman_encode.h", "freeimage317/Source/LibWebP/src/utils/quant_levels.h", "freeimage317/Source/LibWebP/src/utils/quant_levels_dec.h", "freeimage317/Source/LibWebP/src/utils/random.h", "freeimage317/Source/LibWebP/src/utils/rescaler.h", "freeimage317/Source/LibWebP/src/utils/thread.h", "freeimage317/Source/LibWebP/src/utils/utils.h", "freeimage317/Source/LibWebP/src/mux/muxi.h", "freeimage317/Source/LibWebP/src/dec/dec.alpha.c", "freeimage317/Source/LibWebP/src/dec/dec.buffer.c", "freeimage317/Source/LibWebP/src/dec/dec.frame.c", "freeimage317/Source/LibWebP/src/dec/dec.idec.c", "freeimage317/Source/LibWebP/src/dec/dec.io.c", "freeimage317/Source/LibWebP/src/dec/dec.quant.c", "freeimage317/Source/LibWebP/src/dec/dec.tree.c", "freeimage317/Source/LibWebP/src/dec/dec.vp8.c", "freeimage317/Source/LibWebP/src/dec/dec.vp8l.c", "freeimage317/Source/LibWebP/src/dec/dec.webp.c", "freeimage317/Source/LibWebP/src/demux/demux.demux.c", "freeimage317/Source/LibWebP/src/dsp/dsp.alpha_processing.c", "freeimage317/Source/LibWebP/src/dsp/dsp.alpha_processing_mips_dsp_r2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.alpha_processing_sse2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.argb.c", "freeimage317/Source/LibWebP/src/dsp/dsp.argb_mips_dsp_r2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.argb_sse2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.cost.c", "freeimage317/Source/LibWebP/src/dsp/dsp.cost_mips32.c", "freeimage317/Source/LibWebP/src/dsp/dsp.cost_mips_dsp_r2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.cost_sse2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.cpu.c", "freeimage317/Source/LibWebP/src/dsp/dsp.dec.c", "freeimage317/Source/LibWebP/src/dsp/dsp.dec_clip_tables.c", "freeimage317/Source/LibWebP/src/dsp/dsp.dec_mips32.c", "freeimage317/Source/LibWebP/src/dsp/dsp.dec_mips_dsp_r2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.dec_neon.c", "freeimage317/Source/LibWebP/src/dsp/dsp.dec_sse2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.enc.c", "freeimage317/Source/LibWebP/src/dsp/dsp.enc_avx2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.enc_mips32.c", "freeimage317/Source/LibWebP/src/dsp/dsp.enc_mips_dsp_r2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.enc_neon.c", "freeimage317/Source/LibWebP/src/dsp/dsp.enc_sse2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.filters.c", "freeimage317/Source/LibWebP/src/dsp/dsp.filters_mips_dsp_r2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.filters_sse2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.lossless.c", "freeimage317/Source/LibWebP/src/dsp/dsp.lossless_mips32.c", "freeimage317/Source/LibWebP/src/dsp/dsp.lossless_mips_dsp_r2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.lossless_neon.c", "freeimage317/Source/LibWebP/src/dsp/dsp.lossless_sse2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.rescaler.c", "freeimage317/Source/LibWebP/src/dsp/dsp.rescaler_mips32.c", "freeimage317/Source/LibWebP/src/dsp/dsp.rescaler_mips_dsp_r2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.upsampling.c", "freeimage317/Source/LibWebP/src/dsp/dsp.upsampling_mips_dsp_r2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.upsampling_neon.c", "freeimage317/Source/LibWebP/src/dsp/dsp.upsampling_sse2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.yuv.c", "freeimage317/Source/LibWebP/src/dsp/dsp.yuv_mips32.c", "freeimage317/Source/LibWebP/src/dsp/dsp.yuv_mips_dsp_r2.c", "freeimage317/Source/LibWebP/src/dsp/dsp.yuv_sse2.c", "freeimage317/Source/LibWebP/src/enc/enc.alpha.c", "freeimage317/Source/LibWebP/src/enc/enc.analysis.c", "freeimage317/Source/LibWebP/src/enc/enc.backward_references.c", "freeimage317/Source/LibWebP/src/enc/enc.config.c", "freeimage317/Source/LibWebP/src/enc/enc.cost.c", "freeimage317/Source/LibWebP/src/enc/enc.filter.c", "freeimage317/Source/LibWebP/src/enc/enc.frame.c", "freeimage317/Source/LibWebP/src/enc/enc.histogram.c", "freeimage317/Source/LibWebP/src/enc/enc.iterator.c", "freeimage317/Source/LibWebP/src/enc/enc.near_lossless.c", "freeimage317/Source/LibWebP/src/enc/enc.picture.c", "freeimage317/Source/LibWebP/src/enc/enc.picture_csp.c", "freeimage317/Source/LibWebP/src/enc/enc.picture_psnr.c", "freeimage317/Source/LibWebP/src/enc/enc.picture_rescale.c", "freeimage317/Source/LibWebP/src/enc/enc.picture_tools.c", "freeimage317/Source/LibWebP/src/enc/enc.quant.c", "freeimage317/Source/LibWebP/src/enc/enc.syntax.c", "freeimage317/Source/LibWebP/src/enc/enc.token.c", "freeimage317/Source/LibWebP/src/enc/enc.tree.c", "freeimage317/Source/LibWebP/src/enc/enc.vp8l.c", "freeimage317/Source/LibWebP/src/enc/enc.webpenc.c", "freeimage317/Source/LibWebP/src/mux/mux.anim_encode.c", "freeimage317/Source/LibWebP/src/mux/mux.muxedit.c", "freeimage317/Source/LibWebP/src/mux/mux.muxinternal.c", "freeimage317/Source/LibWebP/src/mux/mux.muxread.c", "freeimage317/Source/LibWebP/src/utils/utils.bit_reader.c", "freeimage317/Source/LibWebP/src/utils/utils.bit_writer.c", "freeimage317/Source/LibWebP/src/utils/utils.color_cache.c", "freeimage317/Source/LibWebP/src/utils/utils.filters.c", "freeimage317/Source/LibWebP/src/utils/utils.huffman.c", "freeimage317/Source/LibWebP/src/utils/utils.huffman_encode.c", "freeimage317/Source/LibWebP/src/utils/utils.quant_levels.c", "freeimage317/Source/LibWebP/src/utils/utils.quant_levels_dec.c", "freeimage317/Source/LibWebP/src/utils/utils.random.c", "freeimage317/Source/LibWebP/src/utils/utils.rescaler.c", "freeimage317/Source/LibWebP/src/utils/utils.thread.c", "freeimage317/Source/LibWebP/src/utils/utils.utils.c"
  }

project "zlib"
  kind "StaticLib"
  language "C"

  files {
    "freeimage317/Source/ZLib/adler32.c", "freeimage317/Source/ZLib/compress.c", "freeimage317/Source/ZLib/crc32.c", "freeimage317/Source/ZLib/deflate.c", "freeimage317/Source/ZLib/gzclose.c", "freeimage317/Source/ZLib/gzlib.c", "freeimage317/Source/ZLib/gzread.c", "freeimage317/Source/ZLib/gzwrite.c", "freeimage317/Source/ZLib/infback.c", "freeimage317/Source/ZLib/inffast.c", "freeimage317/Source/ZLib/inflate.c", "freeimage317/Source/ZLib/inftrees.c", "freeimage317/Source/ZLib/trees.c", "freeimage317/Source/ZLib/uncompr.c", "freeimage317/Source/ZLib/zutil.c"
  }
