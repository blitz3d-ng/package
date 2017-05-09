project "png"
  kind "StaticLib"
  language "C"

  includedirs "src/Source/Zlib"

  files {
    "src/Source/LibPNG/png.c", "src/Source/LibPNG/pngerror.c", "src/Source/LibPNG/pngget.c", "src/Source/LibPNG/pngmem.c", "src/Source/LibPNG/pngpread.c", "src/Source/LibPNG/pngread.c", "src/Source/LibPNG/pngrio.c", "src/Source/LibPNG/pngrtran.c", "src/Source/LibPNG/pngrutil.c", "src/Source/LibPNG/pngset.c", "src/Source/LibPNG/pngtrans.c", "src/Source/LibPNG/pngwio.c", "src/Source/LibPNG/pngwrite.c", "src/Source/LibPNG/pngwtran.c", "src/Source/LibPNG/pngwutil.c"
  }
