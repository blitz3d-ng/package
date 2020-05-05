set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED true)

set (CMAKE_EXECUTABLE_SUFFIX_ )

include_directories (SYSTEM ../../modules)

add_executable(blitzcc
  main.cpp
  libs.cpp libs.h
  environ.cpp environ.h
  ex.h

  tree/decl/node.cpp tree/decl/node.h
  tree/decl/data_decl.cpp tree/decl/data_decl.h
  tree/decl/decl_seq.cpp tree/decl/decl_seq.h
  tree/decl/func_decl.cpp tree/decl/func_decl.h
  tree/decl/struct_decl.cpp tree/decl/struct_decl.h
  tree/decl/var_decl.cpp tree/decl/var_decl.h
  tree/decl/vector_decl.cpp tree/decl/vector_decl.h

  tree/stmt/node.cpp tree/stmt/node.h
  tree/stmt/ass.cpp tree/stmt/ass.h
  tree/stmt/decl_stmt.cpp tree/stmt/decl_stmt.h
  tree/stmt/delete.cpp tree/stmt/delete.h
  tree/stmt/delete_each.cpp tree/stmt/delete_each.h
  tree/stmt/dim.cpp tree/stmt/dim.h
  tree/stmt/exit.cpp tree/stmt/exit.h
  tree/stmt/expr_stmt.cpp tree/stmt/expr_stmt.h
  tree/stmt/for.cpp tree/stmt/for.h
  tree/stmt/for_each.cpp tree/stmt/for_each.h
  tree/stmt/gosub.cpp tree/stmt/gosub.h
  tree/stmt/goto.cpp tree/stmt/goto.h
  tree/stmt/if.cpp tree/stmt/if.h
  tree/stmt/include.cpp tree/stmt/include.h
  tree/stmt/insert.cpp tree/stmt/insert.h
  tree/stmt/label.cpp tree/stmt/label.h
  tree/stmt/read.cpp tree/stmt/read.h
  tree/stmt/repeat.cpp tree/stmt/repeat.h
  tree/stmt/restore.cpp tree/stmt/restore.h
  tree/stmt/return.cpp tree/stmt/return.h
  tree/stmt/select.cpp tree/stmt/select.h
  tree/stmt/stmt_seq.cpp tree/stmt/stmt_seq.h
  tree/stmt/while.cpp tree/stmt/while.h

  tree/expr/node.cpp tree/expr/node.h
  tree/expr/after.cpp tree/expr/after.h
  tree/expr/arith_expr.cpp tree/expr/arith_expr.h
  tree/expr/before.cpp tree/expr/before.h
  tree/expr/bin_expr.cpp tree/expr/bin_expr.h
  tree/expr/call.cpp tree/expr/call.h
  tree/expr/cast.cpp tree/expr/cast.h
  tree/expr/expr_seq.cpp tree/expr/expr_seq.h
  tree/expr/first.cpp tree/expr/first.h
  tree/expr/float_const.cpp tree/expr/float_const.h
  tree/expr/int_const.cpp tree/expr/int_const.h
  tree/expr/last.cpp tree/expr/last.h
  tree/expr/new.cpp tree/expr/new.h
  tree/expr/null.cpp tree/expr/null.h
  tree/expr/object_cast.cpp tree/expr/object_cast.h
  tree/expr/object_handle.cpp tree/expr/object_handle.h
  tree/expr/rel_expr.cpp tree/expr/rel_expr.h
  tree/expr/string_const.cpp tree/expr/string_const.h
  tree/expr/uni_expr.cpp tree/expr/uni_expr.h
  tree/expr/var_expr.cpp tree/expr/var_expr.h

  tree/var/node.cpp tree/var/node.h
  tree/var/array_var.cpp tree/var/array_var.h
  tree/var/decl_var.cpp tree/var/decl_var.h
  tree/var/field_var.cpp tree/var/field_var.h
  tree/var/ident_var.cpp tree/var/ident_var.h
  tree/var/vector_var.cpp tree/var/vector_var.h

  tree/prognode.cpp tree/prognode.h

  tree/node.cpp tree/node.h tree/nodes.h
  tree/decl.cpp tree/decl.h
  tree/label.h
  tree/type.cpp tree/type.h
  tree/parser.cpp tree/parser.h
  tree/toker.cpp tree/toker.h

  codegen.h
  codegen_x86/codegen_x86.cpp codegen_x86/codegen_x86.h
  codegen_x86/tile.cpp codegen_x86/tile.h

  assem_x86/asm_insts.cpp assem_x86/assem_x86.cpp assem_x86/assem_x86.h assem_x86/insts.h assem_x86/operand.cpp assem_x86/operand.h
  assem.h

  std.cpp std.h
)

target_include_directories(blitzcc PRIVATE ../src/runtime)
target_link_libraries(blitzcc stdutil nlohmann_json)

if(LLVM_ROOT)
  target_compile_definitions(blitzcc PRIVATE USE_LLVM)

  target_sources(blitzcc PUBLIC
    codegen_llvm/codegen_llvm.h
    codegen_llvm/codegen_llvm.cpp

    linker_lld/linker_lld.h
    linker_lld/linker_lld.cpp
  )

  execute_process(
    COMMAND ${LLVM_ROOT}/bin/llvm-config --cxxflags
    OUTPUT_VARIABLE LLVM_CXX_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND ${LLVM_ROOT}/bin/llvm-config --ldflags --libs
    OUTPUT_VARIABLE LLVM_LD_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  target_compile_options(blitzcc PRIVATE "SHELL:${LLVM_CXX_FLAGS}" -fexceptions -frtti)
  target_link_options(blitzcc PRIVATE "SHELL:${LLVM_LD_FLAGS}")

  target_link_libraries(blitzcc lldCore lldCommon lldDriver lldReaderWriter lldYAML lldMachO lldCOFF lldELF lldMinGW lldWasm)

  # TODO: figure out if this is optional...
  target_link_libraries(blitzcc zlibstatic ncurses)
endif()

if(BB_COVERAGE)
  set_target_properties(blitzcc PROPERTIES COMPILE_FLAGS ${BB_COVERAGE})
  set_target_properties(blitzcc PROPERTIES LINK_FLAGS ${BB_COVERAGE})
endif()
