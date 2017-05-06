project "bblaunch"
  kind "WindowedApp"
  language "C++"

  removeplatforms { "macos", "linux" }

  files { "bblaunch.cpp" }

  filter "platforms:win32 or win64 or mingw32"
    files { "checkdx.cpp", "checkdx.h", "checkie.cpp", "checkie.h", "bblaunch.rc", "resource.h" }
    flags "WinMain"

  targetdir "../_release"
  targetname "Blitz3D"
  targetextension ".exe"

  characterset "Unicode"
  defines "UNICODE" -- needed for mingw32

  links { "dxguid", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "odbc32", "odbccp32" }
