project "ogg"
  language "C"
  kind "StaticLib"

  removeplatforms { "win32", "win64", "mingw32", "linux" }

  includedirs {
    "src/include"
  }

  files {
    "src/src/*.h",
    "src/src/*.c"
  }
