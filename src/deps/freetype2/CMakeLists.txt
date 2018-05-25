add_library(freetype2 STATIC
  src/src/base/ftbase.c
  src/src/base/ftapi.c
  src/src/base/ftbbox.c
  src/src/base/ftbdf.c
  src/src/base/ftbitmap.c
  src/src/base/ftcid.c
  src/src/base/ftdebug.c
  src/src/base/ftfntfmt.c
  src/src/base/ftfstype.c
  src/src/base/ftgasp.c
  src/src/base/ftglyph.c
  src/src/base/ftgxval.c
  src/src/base/ftinit.c
  src/src/base/ftlcdfil.c
  src/src/base/ftmm.c
  src/src/base/ftotval.c
  src/src/base/ftpatent.c
  src/src/base/ftpfr.c
  src/src/base/ftstroke.c
  src/src/base/ftsynth.c
  src/src/base/ftsystem.c
  src/src/base/fttype1.c
  src/src/base/ftwinfnt.c
  src/src/autofit/autofit.c
  src/src/bdf/bdf.c
  src/src/bzip2/ftbzip2.c
  src/src/cache/ftcache.c
  src/src/cff/cff.c
  src/src/cid/type1cid.c
  src/src/gxvalid/gxvalid.c
  src/src/gzip/ftgzip.c
  src/src/lzw/ftlzw.c
  src/src/otvalid/otvalid.c
  src/src/pcf/pcf.c
  src/src/pfr/pfr.c
  src/src/psaux/psaux.c
  src/src/pshinter/pshinter.c
  src/src/psnames/psnames.c
  src/src/raster/raster.c
  src/src/sfnt/sfnt.c
  src/src/smooth/smooth.c
  src/src/truetype/truetype.c
  src/src/type1/type1.c
  src/src/type42/type42.c
  src/src/winfonts/winfnt.c)

target_include_directories(freetype2 SYSTEM PUBLIC src/include)
target_compile_definitions(freetype2 PRIVATE -DFT2_BUILD_LIBRARY)
target_link_libraries(freetype2 ${ZLIB})
