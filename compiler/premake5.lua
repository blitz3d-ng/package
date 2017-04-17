project "compiler"
  kind "ConsoleApp"
  language "C++"

  removeplatforms { "win64" }

  includedirs "../common/include"

  targetdir "../_release/bin"
  targetname "blitzcc"

  files {
    "main.cpp", "libs.cpp", "libs.h",
    "declnode.cpp", "declnode.h", "exprnode.cpp", "exprnode.h", "node.cpp", "node.h", "nodes.h", "prognode.cpp", "prognode.h", "stmtnode.cpp", "stmtnode.h", "varnode.cpp", "varnode.h", "decl.cpp", "decl.h", "environ.cpp", "environ.h", "label.h", "type.cpp", "type.h", "parser.cpp", "parser.h", "toker.cpp", "toker.h",
    "codegen_x86/codegen_x86.cpp", "codegen_x86/codegen_x86.h", "codegen_x86/tile.cpp", "codegen_x86/tile.h",
    "codegen.h",
    "assem_x86/asm_insts.cpp", "assem_x86/assem_x86.cpp", "assem_x86/assem_x86.h", "assem_x86/insts.h", "assem_x86/operand.cpp", "assem_x86/operand.h",
    "assem.h", "ex.h", "std.cpp", "std.h"
  }

  links { "stdutil" }

  filter "platforms:macos or linux"
    buildoptions "-std=c++11"

  filter "platforms:mingw32" -- FIXME: move ScaleBitmap out of stdutil so this isn't needed.
    links "gdi32"

  filter "platforms:win32"
    files { "resource.h", "blitz.rc", "../bbruntime_dll/dpi.manifest" }
