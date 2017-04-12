project "blitzide2"
  kind "WindowedApp"
  language "C++"

  if not os.getenv("WXWIDGETS") then
    removeplatforms { "win32", "win64", "mingw32" }
  end

  characterset "Unicode"

  targetdir "../../_release/bin"
  targetname "ide2"

  files {
    "dpi.h", "dpi.cpp",
    "HtmlHelp.h", "HtmlHelp.cpp",
    "FileView.h", "FileView.cpp",
    "main.cpp"
  }

  filter "platforms:win32 or win64 or mingw32"
    files "blitzide2/blitzide2.rc"

  filter "platforms:win32 or win64"
    flags "WinMain"
    includedirs {
      "C:\\wxWidgets-3.0.2\\include\\msvc",
      "C:\\wxWidgets-3.0.2\\include"
    }

  filter "platforms:win32"
    libdirs "C:\\wxWidgets-3.0.2\\lib\\vc_lib"

  filter "platforms:win64"
    libdirs "C:\\wxWidgets-3.0.2\\lib\\vc_x64_lib"

  filter "platforms:macos or linux"
    buildoptions {
      "-std=c++11",
      "`wx-config --cflags std webview stc`"
    }
    linkoptions "`wx-config --libs std webview stc`"
