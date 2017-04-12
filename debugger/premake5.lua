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
    "debugger/debugger.rc",
    "debugger/debuggerapp.cpp", "debugger/debuggerapp.h",
    "debugger/debugtree.cpp", "debugger/debugtree.h",
    "debugger/mainframe.cpp", "debugger/mainframe.h",
    "debugger/prefs.cpp", "debugger/prefs.h",
    "debugger/sourcefile.cpp", "debugger/sourcefile.h",
    "debugger/stdafx.cpp", "debugger/stdafx.h",
    "debugger/tabber.cpp", "debugger/tabber.h",
    "debugger/dpi.cpp", "debugger/dpi.h",
    "debugger/debugger.h"
  }
