project "assimp.convertutf"
  language "C"
  kind "StaticLib"

  removeplatforms { "win32", "win64", "mingw32" }

  files {
    "src/contrib/ConvertUTF/*.h",
    "src/contrib/ConvertUTF/*.c"
  }
