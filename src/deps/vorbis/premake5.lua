project "vorbisfile"
  language "C"
  kind "StaticLib"

  removeplatforms { "win32", "win64", "mingw32", "linux" }

  includedirs {
    "../ogg/src/include",
    "src/include"
  }

  files {
    "src/lib/*.h",
    "src/lib/*.c"
  }

  removefiles {
    "src/lib/psytune.c",
    "src/lib/vorbisenc.c"
  }
