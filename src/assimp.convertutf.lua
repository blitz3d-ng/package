project "assimp.convertutf"
  language "C"
  kind "StaticLib"

  removeplatforms { "win32", "win64", "mingw32" }

  files {
    "assimp/contrib/ConvertUTF/*.h",
    "assimp/contrib/ConvertUTF/*.c"
  }
