project "debugger.console"
  kind "SharedLib"
  language "C++"

  includedirs "../runtime"

  targetdir "../../_release/bin"

  files "main.cpp"
