project "blitzide"
  kind "WindowedApp"
  language "C++"

  removeplatforms { "macos", "linux", "mingw32" }

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
    "blitzide/about.cpp", "blitzide/about.h",
    "blitzide/blitzide.cpp", "blitzide/blitzide.h",
    "blitzide/editor.cpp", "blitzide/editor.h",
    "blitzide/funclist.cpp", "blitzide/funclist.h",
    "blitzide/htmlhelp.cpp", "blitzide/htmlhelp.h",
    "blitzide/libs.cpp", "blitzide/libs.h",
    "blitzide/mainframe.cpp", "blitzide/mainframe.h",
    "blitzide/prefs.cpp", "blitzide/prefs.h",
    "blitzide/stdafx.cpp", "blitzide/stdafx.h",
    "blitzide/tabber.cpp", "blitzide/tabber.h",
    "blitzide/dpi.cpp", "blitzide/dpi.h",
    "blitzide/blitzide.rc"
  }
