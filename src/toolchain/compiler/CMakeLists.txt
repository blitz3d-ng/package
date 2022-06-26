set (CMAKE_EXECUTABLE_SUFFIX_ )

if(BB64)
  if (DEFINED ENV{LLVM_ROOT})
    set(llvm_path $ENV{LLVM_ROOT})
  else()
    set(llvm_path ${PROJECT_SOURCE_DIR}/_release/toolchains/llvm)
  endif()

  set(LLVM_ROOT ${llvm_path})
  find_package(LLVM 14.0.4 CONFIG PATHS ${LLVM_ROOT})
  find_package(LLD CONFIG PATHS ${LLVM_ROOT})

  # TODO: this will go away once CI is sorted...
  if(NOT LLVM_FOUND OR NOT LLD_FOUND)
    return()
  endif()
endif()

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

  jit_orc/jit_orc.cpp jit_orc/jit_orc.h

  std.cpp std.h
)

if(BB64 AND BB_WINDOWS)
  set_target_properties(blitzcc PROPERTIES OUTPUT_NAME "blitzcc64")
  target_compile_definitions(blitzcc PRIVATE LINKERNAME="linker64" RUNTIMENAME="runtime64")
else()
  target_compile_definitions(blitzcc PRIVATE LINKERNAME="linker" RUNTIMENAME="runtime")
endif()

if(BB_WINDOWS)
  target_compile_definitions(blitzcc PRIVATE LIBSUFFIX="dll" NOMINMAX)
elseif(BB_MACOS)
  target_compile_definitions(blitzcc PRIVATE LIBSUFFIX="dylib")
else()
  target_compile_definitions(blitzcc PRIVATE LIBSUFFIX="so")
endif()

target_include_directories(blitzcc PRIVATE ../src/runtime)
target_link_libraries(blitzcc stdutil nlohmann_json)
target_output_to_bin(blitzcc)

if(LLVM_ROOT)
  target_compile_definitions(blitzcc PRIVATE USE_LLVM)

  target_sources(blitzcc PUBLIC
    codegen_llvm/codegen_llvm.h
    codegen_llvm/codegen_llvm.cpp

    linker_lld/linker_lld.h
    linker_lld/linker_lld.cpp
  )

  target_include_directories(blitzcc PRIVATE ${LLVM_INCLUDE_DIRS})

  llvm_map_components_to_libnames(LLVM_LIBS orcjit)

  if(BB_WINDOWS)
    target_link_libraries(blitzcc lldCOFF lldMinGW)
  elseif(BB_MACOS)
    target_link_libraries(blitzcc lldMachO)
  elseif(BB_LINUX)
    target_link_libraries(blitzcc lldELF)
  endif()
  target_link_libraries(blitzcc ${LLVM_LIBS} zlibstatic)
endif()

if(BB_COVERAGE)
  set_target_properties(blitzcc PROPERTIES COMPILE_FLAGS ${BB_COVERAGE})
  set_target_properties(blitzcc PROPERTIES LINK_FLAGS ${BB_COVERAGE})
endif()
