set(CMAKE_UNITY_BUILD false)
add_compile_options(-w) # suppress warnings

add_library(freetype2 STATIC
  tree/src/base/ftbase.c
  tree/src/base/ftbbox.c
  tree/src/base/ftbdf.c
  tree/src/base/ftbitmap.c
  tree/src/base/ftcid.c
  tree/src/base/ftdebug.c
  tree/src/base/ftfntfmt.c
  tree/src/base/ftfstype.c
  tree/src/base/ftgasp.c
  tree/src/base/ftglyph.c
  tree/src/base/ftgxval.c
  tree/src/base/ftinit.c
  tree/src/base/ftlcdfil.c
  tree/src/base/ftmm.c
  tree/src/base/ftotval.c
  tree/src/base/ftpatent.c
  tree/src/base/ftpfr.c
  tree/src/base/ftstroke.c
  tree/src/base/ftsynth.c
  tree/src/base/ftsystem.c
  tree/src/base/fttype1.c
  tree/src/base/ftwinfnt.c
  tree/src/autofit/autofit.c
  tree/src/bdf/bdf.c
  tree/src/bzip2/ftbzip2.c
  tree/src/cache/ftcache.c
  tree/src/cff/cff.c
  tree/src/cid/type1cid.c
  tree/src/gxvalid/gxvalid.c
  tree/src/gzip/ftgzip.c
  tree/src/lzw/ftlzw.c
  tree/src/otvalid/otvalid.c
  tree/src/pcf/pcf.c
  tree/src/pfr/pfr.c
  tree/src/psaux/psaux.c
  tree/src/pshinter/pshinter.c
  tree/src/psnames/psnames.c
  tree/src/raster/raster.c
  tree/src/sfnt/sfnt.c
  tree/src/smooth/smooth.c
  tree/src/truetype/truetype.c
  tree/src/type1/type1.c
  tree/src/type42/type42.c
  tree/src/winfonts/winfnt.c)

target_output_to_toolchain(freetype2)
target_include_directories(freetype2 SYSTEM PUBLIC tree/include)
target_compile_definitions(freetype2 PRIVATE -DFT2_BUILD_LIBRARY)
target_link_libraries(freetype2 ${ZLIB})
