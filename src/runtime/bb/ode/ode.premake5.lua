project "ode"
  kind "StaticLib"
  language "C++"

  defines {
    "dIDESINGLE",
    "CCD_IDESINGLE",
    "MAC_OS_X_VERSION=1090"
  }

  includedirs {
    "include",
    "src/include",
    "src/ode/src",
    "src/OPCODE",
    "src/libccd/src",
    "src/ou/include"
  }

  removebuildoptions "-std=c++11"

  files {
    "src/ode/src/*.cpp",
    "src/ode/src/*.c",
    "src/ode/src/*.h",

    "src/ode/src/joints/*.cpp",
    "src/ode/src/joints/*.h",

    "src/OPCODE/**/*.cpp",
    "src/OPCODE/**/*.h",

    "src/ou/src/ou/*.cpp"
  }
