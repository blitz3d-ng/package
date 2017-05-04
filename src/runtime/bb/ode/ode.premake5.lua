project "ode"
  kind "StaticLib"
  language "C++"

  defines {
    "dIDESINGLE",
    "CCD_IDESINGLE"
  }

  includedirs {
    "include",
    "src/include",
    "src/ode/src",
    "src/OPCODE",
    "src/libccd/src",
    "src/ou/include"
  }

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
