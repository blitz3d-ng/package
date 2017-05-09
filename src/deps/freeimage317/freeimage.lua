project "freeimage"
  kind "StaticLib"
  language "C++"

  defines "LIBRAW_NODLL"

  includedirs {
    "src/Source",

    "src/Source/LibJXR/jxrgluelib",
    "src/Source/LibJXR/image/sys",
    "src/Source/OpenEXR/Half",
    "src/Source/OpenEXR/Iex",
    "src/Source/OpenEXR/lexMath",
    "src/Source/OpenEXR/IlmImf",
    "src/Source/OpenEXR/IlmThread",
    "src/Source/OpenEXR/Imath",
    "src/Source/OpenEXR",
    "src/Source/Zlib"
  }

  files {
    "src/Source/FreeImage/BitmapAccess.cpp", "src/Source/FreeImage/ColorLookup.cpp", "src/Source/FreeImage/ConversionRGBA16.cpp", "src/Source/FreeImage/ConversionRGBAF.cpp", "src/Source/FreeImage/FreeImage.cpp", "src/Source/FreeImage/FreeImageIO.cpp", "src/Source/FreeImage/GetType.cpp", "src/Source/FreeImage/LFPQuantizer.cpp", "src/Source/FreeImage/MemoryIO.cpp", "src/Source/FreeImage/PixelAccess.cpp", "src/Source/FreeImage/J2KHelper.cpp", "src/Source/FreeImage/MNGHelper.cpp", "src/Source/FreeImage/Plugin.cpp", "src/Source/FreeImage/PluginBMP.cpp", "src/Source/FreeImage/PluginCUT.cpp", "src/Source/FreeImage/PluginDDS.cpp", "src/Source/FreeImage/PluginEXR.cpp", "src/Source/FreeImage/PluginG3.cpp", "src/Source/FreeImage/PluginGIF.cpp", "src/Source/FreeImage/PluginHDR.cpp", "src/Source/FreeImage/PluginICO.cpp", "src/Source/FreeImage/PluginIFF.cpp", "src/Source/FreeImage/PluginJ2K.cpp", "src/Source/FreeImage/PluginJNG.cpp", "src/Source/FreeImage/PluginJP2.cpp", "src/Source/FreeImage/PluginJPEG.cpp", "src/Source/FreeImage/PluginKOALA.cpp", "src/Source/FreeImage/PluginMNG.cpp", "src/Source/FreeImage/PluginPCD.cpp", "src/Source/FreeImage/PluginPCX.cpp", "src/Source/FreeImage/PluginPFM.cpp", "src/Source/FreeImage/PluginPICT.cpp", "src/Source/FreeImage/PluginPNG.cpp", "src/Source/FreeImage/PluginPNM.cpp", "src/Source/FreeImage/PluginPSD.cpp", "src/Source/FreeImage/PluginRAS.cpp", "src/Source/FreeImage/PluginRAW.cpp", "src/Source/FreeImage/PluginSGI.cpp", "src/Source/FreeImage/PluginTARGA.cpp", "src/Source/FreeImage/PluginTIFF.cpp", "src/Source/FreeImage/PluginWBMP.cpp", "src/Source/FreeImage/PluginWebP.cpp", "src/Source/FreeImage/PluginXBM.cpp", "src/Source/FreeImage/PluginXPM.cpp", "src/Source/FreeImage/PSDParser.cpp", "src/Source/FreeImage/TIFFLogLuv.cpp", "src/Source/FreeImage/Conversion.cpp", "src/Source/FreeImage/Conversion16_555.cpp", "src/Source/FreeImage/Conversion16_565.cpp", "src/Source/FreeImage/Conversion24.cpp", "src/Source/FreeImage/Conversion32.cpp", "src/Source/FreeImage/Conversion4.cpp", "src/Source/FreeImage/Conversion8.cpp", "src/Source/FreeImage/ConversionFloat.cpp", "src/Source/FreeImage/ConversionRGB16.cpp", "src/Source/FreeImage/ConversionRGBF.cpp", "src/Source/FreeImage/ConversionType.cpp", "src/Source/FreeImage/ConversionUINT16.cpp", "src/Source/FreeImage/Halftoning.cpp", "src/Source/FreeImage/tmoColorConvert.cpp", "src/Source/FreeImage/tmoDrago03.cpp", "src/Source/FreeImage/tmoFattal02.cpp", "src/Source/FreeImage/tmoReinhard05.cpp", "src/Source/FreeImage/ToneMapping.cpp", "src/Source/FreeImage/NNQuantizer.cpp", "src/Source/FreeImage/WuQuantizer.cpp", "src/Source/DeprecationManager/Deprecated.cpp", "src/Source/DeprecationManager/DeprecationMgr.cpp", "src/Source/FreeImage/CacheFile.cpp", "src/Source/FreeImage/MultiPage.cpp", "src/Source/FreeImage/ZLibInterface.cpp", "src/Source/Metadata/Exif.cpp", "src/Source/Metadata/FIRational.cpp", "src/Source/Metadata/FreeImageTag.cpp", "src/Source/Metadata/IPTC.cpp", "src/Source/Metadata/TagConversion.cpp", "src/Source/Metadata/TagLib.cpp", "src/Source/Metadata/XTIFF.cpp", "src/Source/FreeImageToolkit/Background.cpp", "src/Source/FreeImageToolkit/BSplineRotate.cpp", "src/Source/FreeImageToolkit/Channels.cpp", "src/Source/FreeImageToolkit/ClassicRotate.cpp", "src/Source/FreeImageToolkit/Colors.cpp", "src/Source/FreeImageToolkit/CopyPaste.cpp", "src/Source/FreeImageToolkit/Display.cpp", "src/Source/FreeImageToolkit/Flip.cpp", "src/Source/FreeImageToolkit/JPEGTransform.cpp", "src/Source/FreeImageToolkit/MultigridPoissonSolver.cpp", "src/Source/FreeImageToolkit/Rescale.cpp", "src/Source/FreeImageToolkit/Resize.cpp", "src/Source/CacheFile.h", "src/Source/DeprecationManager/DeprecationMgr.h", "src/Source/MapIntrospector.h", "src/Source/Metadata/FIRational.h", "src/Source/FreeImage.h", "src/Source/FreeImageIO.h", "src/Source/Metadata/FreeImageTag.h", "src/Source/FreeImage/J2KHelper.h", "src/Source/Plugin.h", "src/Source/FreeImage/PSDParser.h", "src/Source/Quantizers.h", "src/Source/ToneMapping.h", "src/Source/Utilities.h", "src/Source/FreeImageToolkit/Resize.h"
  }

  filter "platforms:win32 or win64"
    files {
      "src/Source/FreeImage/PluginJXR.cpp",
      "src/FreeImage.rc"
    }

  filter { "platforms:mingw32 or macos or linux" }
    files "freeimage_jxr_stub.cpp"
