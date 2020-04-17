set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED true)

set (CMAKE_EXECUTABLE_SUFFIX_ )

include_directories (SYSTEM ../src/modules)

add_executable(blitzcc
  main.cpp
  libs.cpp libs.h

  tree/declnode.h tree/declnode.cpp
  tree/declseqnode.h

  tree/exprnode.cpp tree/exprnode.h
  tree/node.cpp tree/node.h tree/nodes.h
  tree/prognode.cpp tree/prognode.h
  tree/stmtnode.cpp tree/stmtnode.h
  tree/varnode.cpp tree/varnode.h
  tree/decl.cpp tree/decl.h
  tree/label.h
  tree/type.cpp tree/type.h
  tree/parser.cpp tree/parser.h
  tree/toker.cpp tree/toker.h

  environ.cpp environ.h

  codegen_x86/codegen_x86.cpp codegen_x86/codegen_x86.h codegen_x86/tile.cpp codegen_x86/tile.h codegen.h

  assem_x86/asm_insts.cpp assem_x86/assem_x86.cpp assem_x86/assem_x86.h assem_x86/insts.h assem_x86/operand.cpp assem_x86/operand.h
  assem.h

  ex.h std.cpp std.h
)

target_include_directories(blitzcc PRIVATE ../src/runtime)
target_link_libraries(blitzcc stdutil nlohmann_json)

if(LLVM_ROOT)
  target_sources(blitzcc PUBLIC codegen_llvm.cpp)

  execute_process(
    COMMAND ${LLVM_ROOT}/bin/llvm-config --cxxflags
    OUTPUT_VARIABLE LLVM_CXX_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND ${LLVM_ROOT}/bin/llvm-config --ldflags --libs
    OUTPUT_VARIABLE SHELL:LLVM_LD_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  target_include_directories(blitzcc PRIVATE ~/llvm-project/llvm/include)
  target_compile_options(blitzcc PRIVATE "SHELL:${LLVM_CXX_FLAGS}" -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING -fexceptions)
  target_link_options(blitzcc PRIVATE "SHELL:${LLVM_LD_FLAGS}")
endif()

if(BB_COVERAGE)
  set_target_properties(blitzcc PROPERTIES COMPILE_FLAGS ${BB_COVERAGE})
  set_target_properties(blitzcc PROPERTIES LINK_FLAGS ${BB_COVERAGE})
endif()
