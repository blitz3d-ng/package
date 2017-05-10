project "glfw"
  kind "StaticLib"
  language "C"

  files {
    "src/src/context.c",
    "src/src/init.c",
    "src/src/input.c",
    "src/src/monitor.c",
    "src/src/vulkan.c",
    "src/src/window.c"
  }

  includedirs {
    "glfw/include"
  }

  filter "platforms:macos"
    defines { "_GLFW_COCOA", "_GLFW_USE_RETINA" }

    files {
      "src/src/cocoa_platform.h",
      "src/src/cocoa_joystick.h",
      "src/src/posix_tls.h",
      "src/src/nsgl_context.h",
      "src/src/cocoa_init.m",
      "src/src/cocoa_joystick.m",
      "src/src/cocoa_monitor.m",
      "src/src/cocoa_window.m",
      "src/src/cocoa_time.c",
      "src/src/posix_tls.c",
      "src/src/nsgl_context.m"
    }

  filter "platforms:win32 or win64 or mingw32"
    defines {
      "_GLFW_WIN32"
    }

    files {
      "src/src/win32_platform.h",
      "src/src/win32_joystick.h",
      "src/src/wgl_context.h",
      "src/src/egl_context.h",

      "src/src/win32_init.c",
      "src/src/win32_joystick.c",
      "src/src/win32_monitor.c",
      "src/src/win32_time.c",
      "src/src/win32_tls.c",
      "src/src/win32_window.c",
      "src/src/wgl_context.c",
      "src/src/egl_context.c"
    }

  filter "platforms:linux"
    defines {
      "_GLFW_X11",
      "_GLFW_HAS_XF86VM"
    }

    files {
      "src/src/egl_context.h",
      "src/src/egl_context.c",
      "src/src/glx_context.h",
      "src/src/glx_context.c",
      "src/src/linux_joystick.h",
      "src/src/linux_joystick.c",
      "src/src/posix_time.h",
      "src/src/posix_time.c",
      "src/src/posix_tls.h",
      "src/src/posix_tls.c",
      "src/src/x11_init.c",
      "src/src/x11_platform.h",
      "src/src/x11_monitor.c",
      "src/src/x11_window.c",
      "src/src/xkb_unicode.h",
      "src/src/xkb_unicode.c",
    }
