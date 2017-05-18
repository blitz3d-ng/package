project "vorbisfile"
  language "C"
  kind "StaticLib"

  removeplatforms { "win32", "win64", "mingw32", "emscripten" }

  includedirs {
    "../ogg/src/include",
    "../ogg/include",
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
