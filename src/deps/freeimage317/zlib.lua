project "zlib"
  kind "StaticLib"
  language "C"

  removeplatforms "emscripten"

  files {
    "src/Source/ZLib/adler32.c", "src/Source/ZLib/compress.c", "src/Source/ZLib/crc32.c", "src/Source/ZLib/deflate.c", "src/Source/ZLib/gzclose.c", "src/Source/ZLib/gzlib.c", "src/Source/ZLib/gzread.c", "src/Source/ZLib/gzwrite.c", "src/Source/ZLib/infback.c", "src/Source/ZLib/inffast.c", "src/Source/ZLib/inflate.c", "src/Source/ZLib/inftrees.c", "src/Source/ZLib/trees.c", "src/Source/ZLib/uncompr.c", "src/Source/ZLib/zutil.c"
  }
