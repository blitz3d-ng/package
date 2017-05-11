project "enet"
  language "C"
  kind "StaticLib"

  includedirs {
    "src/include"
  }

  files {
    "src/include/enet/callbacks.h",
    "src/include/enet/enet.h",
    "src/include/enet/list.h",
    "src/include/enet/protocol.h",
    "src/include/enet/time.h",
    "src/include/enet/types.h",
    "src/include/enet/unix.h",
    "src/include/enet/utility.h",
    "src/include/enet/win32.h",

    "src/callbacks.c",
    "src/compress.c",
    "src/host.c",
    "src/list.c",
    "src/packet.c",
    "src/peer.c",
    "src/protocol.c",
    "src/unix.c",
    "src/win32.c"
  }
