project "glfw"
  kind "StaticLib"
  language "C"

  files {
    "glfw/src/context.c",
    "glfw/src/init.c",
    "glfw/src/input.c",
    "glfw/src/monitor.c",
    "glfw/src/vulkan.c",
    "glfw/src/window.c"
  }

  includedirs {
    "glfw/include"
  }

  filter "platforms:macos"
    defines "_GLFW_COCOA"

  filter "platforms:win32 or win64 or mingw32"
    defines {
      "_GLFW_WIN32"
    }

    files {
      "glfw/src/win32_platform.h",
      "glfw/src/win32_joystick.h",
      "glfw/src/wgl_context.h",
      "glfw/src/egl_context.h",

      "glfw/src/win32_init.c",
      "glfw/src/win32_joystick.c",
      "glfw/src/win32_monitor.c",
      "glfw/src/win32_time.c",
      "glfw/src/win32_tls.c",
      "glfw/src/win32_window.c",
      "glfw/src/wgl_context.c",
      "glfw/src/egl_context.c"
    }

  filter "platforms:linux"
    defines "_GLFW_X11"
