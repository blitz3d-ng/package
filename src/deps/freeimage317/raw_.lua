project "raw"
  kind "StaticLib"
  language "C++"

  removeplatforms "emscripten"

  includedirs "src/Source/LibRawLite"

  defines "LIBRAW_NODLL"

  files {
    "src/Source/LibRawLite/internal/dcraw_common.cpp", "src/Source/LibRawLite/internal/dcraw_fileio.cpp", "src/Source/LibRawLite/internal/demosaic_packs.cpp", "src/Source/LibRawLite/src/libraw_c_api.cpp", "src/Source/LibRawLite/src/libraw_cxx.cpp", "src/Source/LibRawLite/src/libraw_datastream.cpp", "src/Source/LibRawLite/internal/defines.h", "src/Source/LibRawLite/internal/libraw_internal_funcs.h", "src/Source/LibRawLite/internal/var_defines.h", "src/Source/LibRawLite/libraw/libraw.h", "src/Source/LibRawLite/libraw/libraw_alloc.h", "src/Source/LibRawLite/libraw/libraw_const.h", "src/Source/LibRawLite/libraw/libraw_datastream.h", "src/Source/LibRawLite/libraw/libraw_internal.h", "src/Source/LibRawLite/libraw/libraw_types.h", "src/Source/LibRawLite/libraw/libraw_version.h"
  }

  filter "platforms:mingw32 or linux"
    buildoptions "-Wno-narrowing"
