IF(NOT BB_WINDOWS)
  add_library(vorbis STATIC src/lib/analysis.c src/lib/envelope.c src/lib/lpc.c src/lib/psy.c src/lib/smallft.c src/lib/window.c src/lib/floor0.c src/lib/lsp.c src/lib/synthesis.c src/lib/bitrate.c src/lib/floor1.c src/lib/mapping0.c src/lib/registry.c src/lib/block.c src/lib/info.c src/lib/mdct.c src/lib/res0.c src/lib/codebook.c src/lib/lookup.c src/lib/misc.c src/lib/sharedbook.c src/lib/vorbisfile.c src/lib/backends.h src/lib/envelope.h src/lib/lpc.h src/lib/misc.h src/lib/scales.h src/lib/bitrate.h src/lib/highlevel.h src/lib/lsp.h src/lib/os.h src/lib/smallft.h src/lib/codebook.h src/lib/lookup.h src/lib/masking.h src/lib/psy.h src/lib/window.h src/lib/codec_internal.h src/lib/lookup_data.h src/lib/mdct.h src/lib/registry.h)
  target_link_libraries(vorbis PRIVATE ogg)

  target_include_directories(vorbis PUBLIC src/include)
ENDIF()
