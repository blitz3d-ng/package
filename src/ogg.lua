project "ogg"
  language "C"
  kind "StaticLib"

  removeplatforms { "win32", "win64", "mingw32", "linux" }

  includedirs {
    "ogg/include"
  }

  files {
    "ogg/src/*.h",
    "ogg/src/*.c"
  }
