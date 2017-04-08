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
    'filesystem',
    'timer',
    'bank',
    'graphics',
    'input',
    'audio',
    'blitz3d',
    'userlibs'
  }
}

links { "gxruntime" }
links { "audio.fmod", "fmodvc", "system", "system.windows", "filesystem.windows", "input.directinput8" }
links { "timer.windows", "timer" }
links { "blitz", "audio", "bank", "filesystem", "stdutil", "blitz2d", "blitz3d", "graphics", "input", "math", "stream", "string", "sockets" }
links { "freeimage", "jpeg", "jxr", "openexr", "openjpeg", "png", "raw", "tiff4", "webp", "zlib" }
links { "dxguid" }
links { "wsock32", "winmm", "dxguid", "d3dxof", "ddraw", "dinput8", "dsound", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "odbc32", "odbccp32" }

filter "platforms:win32 or win64"
  links "amstrmid"

filter "platforms:mingw32"
  links "strmiids"

return MODULE_CONFIG
