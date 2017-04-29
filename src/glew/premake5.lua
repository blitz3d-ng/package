project "glew"
  language "C"
  kind "StaticLib"

  removeplatforms { "macos", "linux" }

  includedirs "include"

  files "src/glew.c"
