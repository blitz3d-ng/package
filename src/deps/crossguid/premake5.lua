project "crossguid"
  language "C++"
  kind "StaticLib"

  files {
    "src/guid.h",
    "src/guid.cpp"
  }

  filter "platforms:win32 or win64 or mingw32"
    defines "GUID_WINDOWS"

  filter "platforms:macos"
    defines "GUID_CFUUID"

  filter "platforms:linux"
    defines "GUID_LIBUUID"
