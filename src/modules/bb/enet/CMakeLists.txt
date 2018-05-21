add_library (bb.enet STATIC commands.h enet.cpp enet.h)
target_include_directories (bb.enet PRIVATE SYSTEM ../../../deps/enet/src/include)
target_link_libraries (bb.enet bb.blitz bb.hook enet crossguid)
