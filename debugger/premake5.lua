project "debugger"
  kind "SharedLib"
  language "C++"

  removeplatforms { "win64", "mingw32", "macos", "linux" }

  targetdir "../_release/bin"

  buildoptions "/w"

  defines {
    "_WIN32_WINNT=_WIN32_WINNT_WINXP",
    "_USRDLL"
  }

  links "stdutil"

  flags "MFC"

  files {
    "debugger.rc",
    "debuggerapp.cpp", "debuggerapp.h",
    "debugtree.cpp", "debugtree.h",
    "mainframe.cpp", "mainframe.h",
    "prefs.cpp", "prefs.h",
    "sourcefile.cpp", "sourcefile.h",
    "stdafx.cpp", "stdafx.h",
    "tabber.cpp", "tabber.h",
    "dpi.cpp", "dpi.h",
    "debugger.h"
  }
