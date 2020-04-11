set (CMAKE_CXX_STANDARD 11)

set (CMAKE_EXECUTABLE_SUFFIX_ )

include_directories (SYSTEM ../src/modules)

add_executable(blitzcc
  main.cpp
  libs.cpp libs.h
  declnode.cpp declnode.h exprnode.cpp exprnode.h node.cpp node.h nodes.h prognode.cpp prognode.h stmtnode.cpp stmtnode.h varnode.cpp varnode.h decl.cpp decl.h environ.cpp environ.h label.h type.cpp type.h parser.cpp parser.h toker.cpp toker.h
  codegen_x86/codegen_x86.cpp codegen_x86/codegen_x86.h codegen_x86/tile.cpp codegen_x86/tile.h codegen.h
  assem_x86/asm_insts.cpp assem_x86/assem_x86.cpp assem_x86/assem_x86.h assem_x86/insts.h assem_x86/operand.cpp assem_x86/operand.h assem.h ex.h std.cpp std.h
)

target_include_directories(blitzcc PRIVATE ../src/runtime)
target_link_libraries(blitzcc stdutil nlohmann_json)

if(BB_COVERAGE)
  set_target_properties(blitzcc PROPERTIES COMPILE_FLAGS ${BB_COVERAGE})
  set_target_properties(blitzcc PROPERTIES LINK_FLAGS ${BB_COVERAGE})
endif()
