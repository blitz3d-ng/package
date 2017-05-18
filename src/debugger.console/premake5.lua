project "debugger.console"
  kind "SharedLib"
  language "C++"

  removeplatforms "emscripten"

  includedirs "../runtime"

  targetdir "../../_release/bin"

  files "main.cpp"
