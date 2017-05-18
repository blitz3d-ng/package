project "blitzide"
  kind "WindowedApp"
  language "C++"

  removeplatforms { "macos", "linux", "mingw32", "emscripten" }

  targetdir "../_release/bin"
  targetname "ide"
  targetextension ".exe"

  -- characterset "Unicode"

  warnings "Off"
  disablewarnings { "4100","4189" }

  defines {
    "_WIN32_WINNT=_WIN32_WINNT_WINXP",
    "NO_WARN_MBCS_MFC_DEPRECATION"
  }

  flags { "WinMain", "MFC" }

  files {
    "about.cpp", "about.h",
    "blitzide.cpp", "blitzide.h",
    "editor.cpp", "editor.h",
    "funclist.cpp", "funclist.h",
    "htmlhelp.cpp", "htmlhelp.h",
    "libs.cpp", "libs.h",
    "mainframe.cpp", "mainframe.h",
    "prefs.cpp", "prefs.h",
    "stdafx.cpp", "stdafx.h",
    "tabber.cpp", "tabber.h",
    "dpi.cpp", "dpi.h",
    "blitzide.rc"
  }
