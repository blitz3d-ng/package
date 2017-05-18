project "openjpeg"
  kind "StaticLib"
  language "C"

  removeplatforms "emscripten"

  files {
    "src/Source/LibOpenJPEG/bio.c", "src/Source/LibOpenJPEG/cio.c", "src/Source/LibOpenJPEG/dwt.c", "src/Source/LibOpenJPEG/event.c", "src/Source/LibOpenJPEG/function_list.c", "src/Source/LibOpenJPEG/image.c", "src/Source/LibOpenJPEG/invert.c", "src/Source/LibOpenJPEG/j2k.c", "src/Source/LibOpenJPEG/jp2.c", "src/Source/LibOpenJPEG/mct.c", "src/Source/LibOpenJPEG/mqc.c", "src/Source/LibOpenJPEG/openjpeg.c", "src/Source/LibOpenJPEG/opj_clock.c", "src/Source/LibOpenJPEG/pi.c", "src/Source/LibOpenJPEG/raw.c", "src/Source/LibOpenJPEG/t1.c", "src/Source/LibOpenJPEG/t2.c", "src/Source/LibOpenJPEG/tcd.c", "src/Source/LibOpenJPEG/tgt.c", "src/Source/LibOpenJPEG/bio.h", "src/Source/LibOpenJPEG/cio.h", "src/Source/LibOpenJPEG/dwt.h", "src/Source/LibOpenJPEG/event.h", "src/Source/LibOpenJPEG/function_list.h", "src/Source/LibOpenJPEG/image.h", "src/Source/LibOpenJPEG/indexbox_manager.h", "src/Source/LibOpenJPEG/invert.h", "src/Source/LibOpenJPEG/j2k.h", "src/Source/LibOpenJPEG/jp2.h", "src/Source/LibOpenJPEG/mct.h", "src/Source/LibOpenJPEG/mqc.h", "src/Source/LibOpenJPEG/openjpeg.h", "src/Source/LibOpenJPEG/opj_clock.h", "src/Source/LibOpenJPEG/opj_config.h", "src/Source/LibOpenJPEG/opj_config_private.h", "src/Source/LibOpenJPEG/opj_includes.h", "src/Source/LibOpenJPEG/opj_intmath.h", "src/Source/LibOpenJPEG/opj_inttypes.h", "src/Source/LibOpenJPEG/opj_malloc.h", "src/Source/LibOpenJPEG/opj_stdint.h", "src/Source/LibOpenJPEG/pi.h", "src/Source/LibOpenJPEG/raw.h", "src/Source/LibOpenJPEG/t1.h", "src/Source/LibOpenJPEG/t1_luts.h", "src/Source/LibOpenJPEG/t2.h", "src/Source/LibOpenJPEG/tcd.h", "src/Source/LibOpenJPEG/tgt.h"
  }
