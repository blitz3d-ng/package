local MODULE_CONFIG = {
  name = 'Default',
  id = 'default',
  modules = {
    'blitz',
    'math',
    'string',
    'stream',
    'sockets',
    'system',
    'system.windows',
    'filesystem',
    'filesystem.windows',
    'timer',
    'timer.windows',
    'bank',
    'graphics',
    'input',
    'input.directinput8',
    'audio',
    'audio.fmod',
    'blitz3d',
    'userlibs'
  }
}

removeplatforms { "win64", "linux" }

links { "gxruntime", "frame" }
links { "stdutil", "blitz2d" }
links { "freeimage", "jpeg", "jxr", "openexr", "openjpeg", "png", "raw", "tiff4", "webp", "zlib" }

filter "platforms:win32 or win64 or mingw32"
  links { "fmodvc" }
  links { "dxguid" }
  links { "wsock32", "winmm", "dxguid", "d3dxof", "ddraw", "dinput8", "dsound", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "odbc32", "odbccp32" }

filter "platforms:win32 or win64"
  links "amstrmid"

filter "platforms:mingw32"
  links "strmiids"

return MODULE_CONFIG
