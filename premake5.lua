-- premake5.lua
workspace "blitz3d"
  configurations { "debug" } --, "release" }
  platforms { "win32" } --, "macos", "linux" }

  location "build"

  characterset "MBCS"
  architecture "x86"
  exceptionhandling "SEH"

  defines {
    "PRO", -- "MEMDEBUG",
    "FREEIMAGE_LIB",
    "_CRT_SECURE_NO_WARNINGS",
    "DIRECTSOUND_VERSION=0x700",
    "DIRECTINPUT_VERSION=0x700"
  }

  buildoptions {
    "/Gy" -- function level linking: true
  }
  callingconvention "StdCall"

  -- flags "StaticRuntime"

  includedirs {
    "../freeimage317/Source",
  }
  libdirs "../fmodapi375win/api/lib"

  -- DirectX SDK
  includedirs "../dx7sdk/include"
  libdirs "../dx7sdk/lib"
  -- includedirs "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include"
  -- libdirs "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86"

  -- FIXME: we should attend to the warnings at some point...
  -- buildoptions "/w"

  filter "kind:StaticLib"
    defines "_LIB"

  filter "configurations:debug"
    symbols "on"

    defines { "DEBUG" }

project "blitzide"
  kind "WindowedApp"
  language "C++"

  targetdir "_release/bin"
  targetname "ide"
  targetextension ".exe"

  defines {
    "_WIN32_WINNT=_WIN32_WINNT_WINXP"
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

-- project "blitzide2"
--   kind "WindowedApp"
--   language "C++"
--
--   targetdir "_release/bin"
--   targetname "ide"
--
--   buildoptions {
--     "-std=c++11",
--     "`wx-config --cflags std webview stc`"
--   }
--   linkoptions "`wx-config --libs std webview stc`"
--
--   files {
--     "blitzide2/HtmlHelp.h", "blitzide2/HtmlHelp.cpp",
--     "blitzide2/FileView.h", "blitzide2/FileView.cpp",
--     "blitzide2/main.cpp"
--   }

project "bblaunch"
  kind "WindowedApp"
  language "C++"

  files { "bblaunch/bblaunch.cpp" }

  filter "platforms:win32"
    files { "bblaunch/checkdx.cpp", "bblaunch/checkdx.h", "bblaunch/checkie.cpp", "bblaunch/checkie.h", "bblaunch/bblaunch.rc", "re" }
    flags "WinMain"

  targetdir "_release"
  targetname "Blitz3D"
  targetextension ".exe"

  -- linkoptions "/force"

  links { "dxguid", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "odbc32", "odbccp32" }

project "bbruntime_dll"
  kind "SharedLib"
  language "C++"

  targetdir "_release/bin"
  targetprefix ""
  targetname "runtime"

  -- linkoptions "/force"

  files {
    "bbruntime_dll/bbruntime_dll.h",
    "bbruntime_dll/bbruntime_dll.cpp",
    "bbruntime_dll/bbruntime_dll.rc",
    "bbruntime_dll/resource.h",
    "bbruntime_dll/dpi.manifest"
  }

  links {
    "bbruntime",
    "stdutil",
  }

  links { "dxguid" }
  links { "blitz3d", "gxruntime", "fmodvc", "freeimage" }
  links { "wsock32", "amstrmid", "winmm", "dxguid", "d3dxof", "dplayx", "ddraw", "dinput", "dsound", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "odbc32", "odbccp32" }

project "gxruntime"
  kind "StaticLib"
  language "C++"

  files { "gxruntime/ddutil.cpp", "gxruntime/gxaudio.cpp", "gxruntime/gxcanvas.cpp", "gxruntime/gxchannel.cpp", "gxruntime/gxdevice.cpp", "gxruntime/gxdir.cpp", "gxruntime/gxfilesystem.cpp", "gxruntime/gxfont.cpp", "gxruntime/gxgraphics.cpp", "gxruntime/gxinput.cpp", "gxruntime/gxlight.cpp", "gxruntime/gxmesh.cpp", "gxruntime/gxmovie.cpp", "gxruntime/gxruntime.cpp", "gxruntime/gxscene.cpp", "gxruntime/gxsound.cpp", "gxruntime/gxtimer.cpp", "gxruntime/std.cpp", "gxruntime/asmcoder.h", "gxruntime/ddutil.h", "gxruntime/gxaudio.h", "gxruntime/gxcanvas.h", "gxruntime/gxchannel.h", "gxruntime/gxdevice.h", "gxruntime/gxdir.h", "gxruntime/gxfilesystem.h", "gxruntime/gxfont.h", "gxruntime/gxgraphics.h", "gxruntime/gxinput.h", "gxruntime/gxlight.h", "gxruntime/gxmesh.h", "gxruntime/gxmovie.h", "gxruntime/gxruntime.h", "gxruntime/gxscene.h", "gxruntime/gxsound.h", "gxruntime/gxtimer.h", "gxruntime/std.h" }

project "bbruntime"
  kind "StaticLib"
  language "C++"

  files {
    "bbruntime/basic.cpp",
    "bbruntime/bbaudio.cpp",
    "bbruntime/bbbank.cpp",
    "bbruntime/bbblitz3d.cpp",
    "bbruntime/bbfilesystem.cpp",
    "bbruntime/bbgraphics.cpp",
    "bbruntime/bbinput.cpp",
    "bbruntime/bbmath.cpp",
    "bbruntime/bbruntime.cpp",
    "bbruntime/bbsockets.cpp",
    "bbruntime/bbstream.cpp",
    "bbruntime/bbstring.cpp",
    "bbruntime/bbsys.cpp",
    "bbruntime/multiplay.cpp",
    "bbruntime/multiplay_setup.cpp",
    "bbruntime/std.cpp",
    "bbruntime/userlibs.cpp",
    "bbruntime/basic.h",
    "bbruntime/bbaudio.h",
    "bbruntime/bbbank.h",
    "bbruntime/bbblitz3d.h",
    "bbruntime/bbfilesystem.h",
    "bbruntime/bbgraphics.h",
    "bbruntime/bbinput.h",
    "bbruntime/bbmath.h",
    "bbruntime/bbruntime.h",
    "bbruntime/bbsockets.h",
    "bbruntime/bbstream.h",
    "bbruntime/bbstring.h",
    "bbruntime/bbsys.h",
    "bbruntime/multiplay.h",
    "bbruntime/multiplay_setup.h",
    "bbruntime/resource.h",
    "bbruntime/std.h",
    "bbruntime/userlibs.h"
  }

project "blitz3d"
  kind "StaticLib"
  language "C++"

  files {
    "blitz3d/animation.cpp", "blitz3d/animator.cpp", "blitz3d/brush.cpp", "blitz3d/cachedtexture.cpp", "blitz3d/camera.cpp", "blitz3d/collision.cpp", "blitz3d/entity.cpp", "blitz3d/frustum.cpp", "blitz3d/geom.cpp", "blitz3d/light.cpp", "blitz3d/listener.cpp", "blitz3d/loader_3ds.cpp", "blitz3d/loader_b3d.cpp", "blitz3d/loader_x.cpp", "blitz3d/md2model.cpp", "blitz3d/md2norms.cpp", "blitz3d/md2rep.cpp", "blitz3d/meshcollider.cpp", "blitz3d/meshloader.cpp", "blitz3d/meshmodel.cpp", "blitz3d/meshutil.cpp", "blitz3d/mirror.cpp", "blitz3d/model.cpp", "blitz3d/object.cpp", "blitz3d/pivot.cpp", "blitz3d/planemodel.cpp", "blitz3d/q3bspmodel.cpp", "blitz3d/q3bsprep.cpp", "blitz3d/sprite.cpp", "blitz3d/std.cpp", "blitz3d/surface.cpp", "blitz3d/terrain.cpp", "blitz3d/terrainrep.cpp", "blitz3d/texture.cpp", "blitz3d/world.cpp", "blitz3d/animation.h", "blitz3d/animator.h", "blitz3d/blitz3d.h", "blitz3d/brush.h", "blitz3d/cachedtexture.h", "blitz3d/camera.h", "blitz3d/collision.h", "blitz3d/entity.h", "blitz3d/frustum.h", "blitz3d/geom.h", "blitz3d/light.h", "blitz3d/listener.h", "blitz3d/loader_3ds.h", "blitz3d/loader_b3d.h", "blitz3d/loader_x.h", "blitz3d/md2model.h", "blitz3d/md2norms.h", "blitz3d/md2rep.h", "blitz3d/meshcollider.h", "blitz3d/meshloader.h", "blitz3d/meshmodel.h", "blitz3d/meshutil.h", "blitz3d/mirror.h", "blitz3d/model.h", "blitz3d/object.h", "blitz3d/pivot.h", "blitz3d/planemodel.h", "blitz3d/q3bspmodel.h", "blitz3d/q3bsprep.h", "blitz3d/rendercontext.h", "blitz3d/sprite.h", "blitz3d/std.h", "blitz3d/surface.h", "blitz3d/terrain.h", "blitz3d/terrainrep.h", "blitz3d/texture.h", "blitz3d/world.h"
  }

project "blitz"
  kind "ConsoleApp"
  language "C++"

  files { "blitz/main.cpp", "blitz/libs.cpp" }

  targetdir "_release/bin"
  targetname "blitzcc"

  links { "compiler", "linker", "stdutil" }
  linkoptions { "-mconsole" }

  filter "platforms:win32"
    files "bbruntime_dll/dpi.manifest"


project "compiler"
  kind "StaticLib"
  language "C++"

  files {
    "compiler/declnode.cpp", "compiler/declnode.h", "compiler/exprnode.cpp", "compiler/exprnode.h", "compiler/node.cpp", "compiler/node.h", "compiler/nodes.h", "compiler/prognode.cpp", "compiler/prognode.h", "compiler/stmtnode.cpp", "compiler/stmtnode.h", "compiler/varnode.cpp", "compiler/varnode.h", "compiler/decl.cpp", "compiler/decl.h", "compiler/environ.cpp", "compiler/environ.h", "compiler/label.h", "compiler/type.cpp", "compiler/type.h", "compiler/parser.cpp", "compiler/parser.h", "compiler/toker.cpp", "compiler/toker.h",
    "compiler/codegen_x86/codegen_x86.cpp", "compiler/codegen_x86/codegen_x86.h", "compiler/codegen_x86/tile.cpp", "compiler/codegen_x86/tile.h",
    "compiler/codegen.h",
    "compiler/assem_x86/asm_insts.cpp", "compiler/assem_x86/assem_x86.cpp", "compiler/assem_x86/assem_x86.h", "compiler/assem_x86/insts.h", "compiler/assem_x86/operand.cpp", "compiler/assem_x86/operand.h",
    "compiler/assem.h", "compiler/ex.h", "compiler/std.cpp", "compiler/std.h"
  }

project "linker_dll"
  kind "SharedLib"
  language "C++"

  targetdir "_release/bin"
  targetname "linker"

  linkoptions { "-shared" }

  files { "linker_dll/linker_dll.h", "linker_dll/linker_dll.cpp" }
  links { "linker", "stdutil" }

project "linker"
  kind "StaticLib"
  language "C++"

  files { "linker/linker.h", "linker/linker.cpp", "linker/image_util.h", "linker/image_util.cpp" }

project "stdutil"
  kind "StaticLib"
  language "C++"

  files { "stdutil/stdutil.h", "stdutil/stdutil.cpp" }

project "freeimage"
  kind "StaticLib"
  language "C++"

  includedirs {
    "../freeimage317/Source/Zlib"
  }

  callingconvention "Cdecl"

  defines { "FREEIMAGE_LIBRARY_TYPE=STATIC" }

  files {
    "../freeimage317/Source/FreeImage/BitmapAccess.cpp", "../freeimage317/Source/FreeImage/ColorLookup.cpp", "../freeimage317/Source/FreeImage/FreeImage.cpp", "../freeimage317/Source/FreeImage/FreeImageC.c", "../freeimage317/Source/FreeImage/FreeImageIO.cpp", "../freeimage317/Source/FreeImage/GetType.cpp", "../freeimage317/Source/FreeImage/MemoryIO.cpp", "../freeimage317/Source/FreeImage/PixelAccess.cpp", "../freeimage317/Source/FreeImage/MNGHelper.cpp", "../freeimage317/Source/FreeImage/Plugin.cpp", "../freeimage317/Source/FreeImage/PluginBMP.cpp", "../freeimage317/Source/FreeImage/PluginCUT.cpp", "../freeimage317/Source/FreeImage/PluginDDS.cpp", "../freeimage317/Source/FreeImage/PluginGIF.cpp", "../freeimage317/Source/FreeImage/PluginHDR.cpp", "../freeimage317/Source/FreeImage/PluginICO.cpp", "../freeimage317/Source/FreeImage/PluginIFF.cpp", "../freeimage317/Source/FreeImage/PluginJNG.cpp", "../freeimage317/Source/FreeImage/PluginJPEG.cpp", "../freeimage317/Source/FreeImage/PluginKOALA.cpp", "../freeimage317/Source/FreeImage/PluginMNG.cpp", "../freeimage317/Source/FreeImage/PluginPCD.cpp", "../freeimage317/Source/FreeImage/PluginPCX.cpp", "../freeimage317/Source/FreeImage/PluginPFM.cpp", "../freeimage317/Source/FreeImage/PluginPICT.cpp", "../freeimage317/Source/FreeImage/PluginPNG.cpp", "../freeimage317/Source/FreeImage/PluginPNM.cpp", "../freeimage317/Source/FreeImage/PluginPSD.cpp", "../freeimage317/Source/FreeImage/PluginRAS.cpp", "../freeimage317/Source/FreeImage/PluginSGI.cpp", "../freeimage317/Source/FreeImage/PluginTARGA.cpp",  "../freeimage317/Source/FreeImage/PluginWBMP.cpp", "../freeimage317/Source/FreeImage/PluginXBM.cpp", "../freeimage317/Source/FreeImage/PluginXPM.cpp", "../freeimage317/Source/FreeImage/PSDParser.cpp", "../freeimage317/Source/FreeImage/TIFFLogLuv.cpp", "../freeimage317/Source/FreeImage/Conversion.cpp", "../freeimage317/Source/FreeImage/Conversion16_555.cpp", "../freeimage317/Source/FreeImage/Conversion16_565.cpp", "../freeimage317/Source/FreeImage/Conversion24.cpp", "../freeimage317/Source/FreeImage/Conversion32.cpp", "../freeimage317/Source/FreeImage/Conversion4.cpp", "../freeimage317/Source/FreeImage/Conversion8.cpp", "../freeimage317/Source/FreeImage/ConversionFloat.cpp", "../freeimage317/Source/FreeImage/ConversionRGB16.cpp", "../freeimage317/Source/FreeImage/ConversionRGBA16.cpp", "../freeimage317/Source/FreeImage/ConversionRGBAF.cpp", "../freeimage317/Source/FreeImage/ConversionRGBF.cpp", "../freeimage317/Source/FreeImage/ConversionType.cpp", "../freeimage317/Source/FreeImage/ConversionUINT16.cpp", "../freeimage317/Source/FreeImage/Halftoning.cpp", "../freeimage317/Source/FreeImage/tmoColorConvert.cpp", "../freeimage317/Source/FreeImage/tmoDrago03.cpp", "../freeimage317/Source/FreeImage/tmoFattal02.cpp", "../freeimage317/Source/FreeImage/tmoReinhard05.cpp", "../freeimage317/Source/FreeImage/ToneMapping.cpp", "../freeimage317/Source/FreeImage/LFPQuantizer.cpp", "../freeimage317/Source/FreeImage/NNQuantizer.cpp", "../freeimage317/Source/FreeImage/WuQuantizer.cpp", "../freeimage317/Source/DeprecationManager/Deprecated.cpp", "../freeimage317/Source/DeprecationManager/DeprecationMgr.cpp", "../freeimage317/Source/FreeImage/CacheFile.cpp", "../freeimage317/Source/FreeImage/MultiPage.cpp", "../freeimage317/Source/FreeImage/ZLibInterface.cpp", "../freeimage317/Source/Metadata/Exif.cpp", "../freeimage317/Source/Metadata/FIRational.cpp", "../freeimage317/Source/Metadata/FreeImageTag.cpp", "../freeimage317/Source/Metadata/IPTC.cpp", "../freeimage317/Source/Metadata/TagConversion.cpp", "../freeimage317/Source/Metadata/TagLib.cpp", "../freeimage317/Source/FreeImageToolkit/Background.cpp", "../freeimage317/Source/FreeImageToolkit/BSplineRotate.cpp", "../freeimage317/Source/FreeImageToolkit/Channels.cpp", "../freeimage317/Source/FreeImageToolkit/ClassicRotate.cpp", "../freeimage317/Source/FreeImageToolkit/Colors.cpp", "../freeimage317/Source/FreeImageToolkit/CopyPaste.cpp", "../freeimage317/Source/FreeImageToolkit/Display.cpp", "../freeimage317/Source/FreeImageToolkit/Flip.cpp", "../freeimage317/Source/FreeImageToolkit/JPEGTransform.cpp", "../freeimage317/Source/FreeImageToolkit/MultigridPoissonSolver.cpp", "../freeimage317/Source/FreeImageToolkit/Rescale.cpp", "../freeimage317/Source/FreeImageToolkit/Resize.cpp",
    "../freeimage317/Source/LibJPEG/./jaricom.c", "../freeimage317/Source/LibJPEG/jcapimin.c", "../freeimage317/Source/LibJPEG/jcapistd.c", "../freeimage317/Source/LibJPEG/./jcarith.c", "../freeimage317/Source/LibJPEG/jccoefct.c", "../freeimage317/Source/LibJPEG/jccolor.c", "../freeimage317/Source/LibJPEG/jcdctmgr.c", "../freeimage317/Source/LibJPEG/jchuff.c", "../freeimage317/Source/LibJPEG/jcinit.c", "../freeimage317/Source/LibJPEG/jcmainct.c", "../freeimage317/Source/LibJPEG/jcmarker.c", "../freeimage317/Source/LibJPEG/jcmaster.c", "../freeimage317/Source/LibJPEG/jcomapi.c", "../freeimage317/Source/LibJPEG/jcparam.c", "../freeimage317/Source/LibJPEG/jcprepct.c", "../freeimage317/Source/LibJPEG/jcsample.c", "../freeimage317/Source/LibJPEG/jctrans.c", "../freeimage317/Source/LibJPEG/jdapimin.c", "../freeimage317/Source/LibJPEG/jdapistd.c", "../freeimage317/Source/LibJPEG/./jdarith.c", "../freeimage317/Source/LibJPEG/jdatadst.c", "../freeimage317/Source/LibJPEG/jdatasrc.c", "../freeimage317/Source/LibJPEG/jdcoefct.c", "../freeimage317/Source/LibJPEG/jdcolor.c", "../freeimage317/Source/LibJPEG/jddctmgr.c", "../freeimage317/Source/LibJPEG/jdhuff.c", "../freeimage317/Source/LibJPEG/jdinput.c", "../freeimage317/Source/LibJPEG/jdmainct.c", "../freeimage317/Source/LibJPEG/jdmarker.c", "../freeimage317/Source/LibJPEG/jdmaster.c", "../freeimage317/Source/LibJPEG/jdmerge.c", "../freeimage317/Source/LibJPEG/jdpostct.c", "../freeimage317/Source/LibJPEG/jdsample.c", "../freeimage317/Source/LibJPEG/jdtrans.c", "../freeimage317/Source/LibJPEG/jerror.c", "../freeimage317/Source/LibJPEG/jfdctflt.c", "../freeimage317/Source/LibJPEG/jfdctfst.c", "../freeimage317/Source/LibJPEG/jfdctint.c", "../freeimage317/Source/LibJPEG/jidctflt.c", "../freeimage317/Source/LibJPEG/jidctfst.c", "../freeimage317/Source/LibJPEG/jidctint.c", "../freeimage317/Source/LibJPEG/jmemmgr.c", "../freeimage317/Source/LibJPEG/jmemnobs.c", "../freeimage317/Source/LibJPEG/jquant1.c", "../freeimage317/Source/LibJPEG/jquant2.c",
    "../freeimage317/Source/LibJPEG/jutils.c", "../freeimage317/Source/LibJPEG/transupp.c", "../freeimage317/Source/LibPNG/./png.c", "../freeimage317/Source/LibPNG/./pngerror.c", "../freeimage317/Source/LibPNG/./pngget.c", "../freeimage317/Source/LibPNG/./pngmem.c", "../freeimage317/Source/LibPNG/./pngpread.c", "../freeimage317/Source/LibPNG/./pngread.c", "../freeimage317/Source/LibPNG/./pngrio.c", "../freeimage317/Source/LibPNG/./pngrtran.c", "../freeimage317/Source/LibPNG/./pngrutil.c", "../freeimage317/Source/LibPNG/./pngset.c", "../freeimage317/Source/LibPNG/./pngtrans.c", "../freeimage317/Source/LibPNG/./pngwio.c", "../freeimage317/Source/LibPNG/./pngwrite.c",
    "../freeimage317/Source/LibPNG/./pngwtran.c", "../freeimage317/Source/LibPNG/./pngwutil.c",
    "../freeimage317/Source/ZLib/./adler32.c", "../freeimage317/Source/ZLib/./compress.c", "../freeimage317/Source/ZLib/./crc32.c", "../freeimage317/Source/ZLib/./deflate.c", "../freeimage317/Source/ZLib/./gzclose.c", "../freeimage317/Source/ZLib/./gzlib.c", "../freeimage317/Source/ZLib/./gzread.c", "../freeimage317/Source/ZLib/./gzwrite.c", "../freeimage317/Source/ZLib/./infback.c", "../freeimage317/Source/ZLib/./inffast.c", "../freeimage317/Source/ZLib/./inflate.c", "../freeimage317/Source/ZLib/./inftrees.c", "../freeimage317/Source/ZLib/./trees.c", "../freeimage317/Source/ZLib/./uncompr.c", "../freeimage317/Source/ZLib/./zutil.c"
  }
