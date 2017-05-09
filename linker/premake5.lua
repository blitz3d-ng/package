project "linker"
  kind "SharedLib"
  language "C++"

  removeplatforms { "win64", "macos", "linux" }

  targetdir "../_release/bin"

  includedirs "../src/runtime"

  files { "main.cpp", "linker.h", "linker.cpp", "image_util.h", "image_util.cpp" }
  links { "stdutil" }

  filter "platforms:mingw32" -- FIXME: move ScaleBitmap out of stdutil so this isn't needed.
    links "gdi32"
