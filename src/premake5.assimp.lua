project "assimp"
  language "C++"
  kind "StaticLib"

  removeplatforms { "win32", "win64", "mingw32" }

  includedirs {
  --   "assimp",
  --   "assimp/contrib/zlib/",
    "assimp/include"
  }

  files {
    "assimp/code/XFileHelper.h",
    "assimp/code/XFileImporter.cpp",
    "assimp/code/XFileImporter.h",
    "assimp/code/XFileParser.cpp",
    "assimp/code/XFileParser.h",
    "assimp/code/XFileExporter.h",
    "assimp/code/XFileExporter.cpp"
  }
