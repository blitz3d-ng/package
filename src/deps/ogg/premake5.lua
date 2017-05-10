project "ogg"
  language "C"
  kind "StaticLib"

  removeplatforms { "win32", "win64", "mingw32" }

  includedirs {
    "include",
    "src/include"
  }

  files {
    "src/src/*.h",
    "src/src/*.c"
  }
