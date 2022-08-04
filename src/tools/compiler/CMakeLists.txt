set (CMAKE_EXECUTABLE_SUFFIX_ )

if(BB64)
  set(LLVM_KNOWN_PATHS
    $ENV{LLVM_ROOT}
    ${PROJECT_SOURCE_DIR}/llvm
    /usr/lib/llvm-14/lib/cmake/lld
    c:/llvm-x64
    c:/llvm-14
    c:/llvm
  )

  find_package(LLVM 14 PATHS ${LLVM_KNOWN_PATHS})
  find_package(LLD PATHS ${LLVM_KNOWN_PATHS})

  # TODO: this will go away once CI is sorted...
  if(NOT LLVM_FOUND OR NOT LLD_FOUND)
    return()
  endif()
endif()

include_directories (SYSTEM ../../modules)

set(TREE_SOURCES
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
)

add_executable(blitzcc
  main.cpp
  libs.cpp libs.h
  environ.cpp environ.h
  ex.h

  ${TREE_SOURCES}

  codegen.h
  codegen_x86/codegen_x86.cpp codegen_x86/codegen_x86.h
  codegen_x86/tile.cpp codegen_x86/tile.h

  assem_x86/asm_insts.cpp assem_x86/assem_x86.cpp assem_x86/assem_x86.h assem_x86/insts.h assem_x86/operand.cpp assem_x86/operand.h
  assem.h

  linker_x86/linker.h linker_x86/linker.cpp linker_x86/image_util.h linker_x86/image_util.cpp

  std.cpp std.h
)

set_target_properties(blitzcc PROPERTIES UNITY_BUILD_MODE GROUP)
set_source_files_properties(${TREE_SOURCES} PROPERTIES UNITY_GROUP "tree")

if(BB64 AND BB_WINDOWS)
  set_target_properties(blitzcc PROPERTIES OUTPUT_NAME "blitzcc64")
  target_compile_definitions(blitzcc PRIVATE RUNTIMENAME="runtime64")
else()
  target_compile_definitions(blitzcc PRIVATE RUNTIMENAME="runtime")
endif()

if(BB_WINDOWS)
  target_compile_definitions(blitzcc PRIVATE LIBSUFFIX="dll" NOMINMAX)
elseif(BB_MACOS)
  target_compile_definitions(blitzcc PRIVATE LIBSUFFIX="dylib")
else()
  target_compile_definitions(blitzcc PRIVATE LIBSUFFIX="so")
endif()

if(BB_LINUX AND BB_GNU)
  set(LIBARCH_DIR /usr/lib/x86_64-linux-gnu) # TODO: should find a way to derive this... `gcc -v` + parsing is an option.
  execute_process(COMMAND gcc --print-file-name= OUTPUT_VARIABLE LIBGCC_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)

  target_compile_definitions(blitzcc PRIVATE LIBARCH_DIR="${LIBARCH_DIR}" LIBGCC_DIR="${LIBGCC_DIR}")
endif()

target_include_directories(blitzcc PRIVATE ../src/runtime)
target_link_libraries(blitzcc stdutil nlohmann_json)
target_output_to_bin(blitzcc)

if(LLVM_FOUND)
  target_compile_definitions(blitzcc PRIVATE USE_LLVM)

  target_sources(blitzcc PUBLIC
    codegen_llvm/codegen_llvm.h
    codegen_llvm/codegen_llvm.cpp

    linker_lld/linker_lld.h
    linker_lld/linker_lld.cpp

    package/bundle.cpp
    package/package_apk.cpp
    package/package.h

    jit_orc/jit_orc.cpp jit_orc/jit_orc.h
  )

  if(BB_MACOS)
    target_sources(blitzcc PUBLIC package/package_app.cpp)
  endif()

  target_include_directories(blitzcc PRIVATE ${LLVM_INCLUDE_DIRS})

  llvm_map_components_to_libnames(LLVM_LIBS orcjit)

  if(BB_WINDOWS)
    if(BB_MSVC)
      target_compile_options(blitzcc PRIVATE /wd4624)
    endif()

    target_link_libraries(blitzcc lldCOFF lldMinGW)
  elseif(BB_MACOS)
    target_link_libraries(blitzcc lldMachO)
  endif()
  target_link_libraries(blitzcc lldWasm lldELF ${LLVM_LIBS} zlibstatic)
endif()

if(BB_COVERAGE)
  set_target_properties(blitzcc PROPERTIES COMPILE_FLAGS ${BB_COVERAGE})
  set_target_properties(blitzcc PROPERTIES LINK_FLAGS ${BB_COVERAGE})
endif()
