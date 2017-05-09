project "vorbisfile"
  language "C"
  kind "StaticLib"

  removeplatforms { "win32", "win64", "mingw32", "linux" }

  includedirs {
    "ogg/include",
    "vorbis/include"
  }

  files {
    "vorbis/lib/*.h",
    "vorbis/lib/*.c"
  }

  removefiles {
    "vorbis/lib/psytune.c",
    "vorbis/lib/vorbisenc.c"
  }
