project "tiff4"
  kind "StaticLib"
  language "C"

  removeplatforms "emscripten"

  defines 'TIFF_SSIZE_FORMAT=""'

  files {
    "src/Source/LibTIFF4/tif_aux.c", "src/Source/LibTIFF4/tif_close.c", "src/Source/LibTIFF4/tif_codec.c", "src/Source/LibTIFF4/tif_color.c", "src/Source/LibTIFF4/tif_compress.c", "src/Source/LibTIFF4/tif_dir.c", "src/Source/LibTIFF4/tif_dirinfo.c", "src/Source/LibTIFF4/tif_dirread.c", "src/Source/LibTIFF4/tif_dirwrite.c", "src/Source/LibTIFF4/tif_dumpmode.c", "src/Source/LibTIFF4/tif_error.c", "src/Source/LibTIFF4/tif_extension.c", "src/Source/LibTIFF4/tif_fax3.c", "src/Source/LibTIFF4/tif_fax3sm.c", "src/Source/LibTIFF4/tif_flush.c", "src/Source/LibTIFF4/tif_getimage.c", "src/Source/LibTIFF4/tif_jpeg.c", "src/Source/LibTIFF4/tif_luv.c", "src/Source/LibTIFF4/tif_lzma.c", "src/Source/LibTIFF4/tif_lzw.c", "src/Source/LibTIFF4/tif_next.c", "src/Source/LibTIFF4/tif_ojpeg.c", "src/Source/LibTIFF4/tif_open.c", "src/Source/LibTIFF4/tif_packbits.c", "src/Source/LibTIFF4/tif_pixarlog.c", "src/Source/LibTIFF4/tif_predict.c", "src/Source/LibTIFF4/tif_print.c", "src/Source/LibTIFF4/tif_read.c", "src/Source/LibTIFF4/tif_strip.c", "src/Source/LibTIFF4/tif_swab.c", "src/Source/LibTIFF4/tif_thunder.c", "src/Source/LibTIFF4/tif_tile.c", "src/Source/LibTIFF4/tif_version.c", "src/Source/LibTIFF4/tif_warning.c", "src/Source/LibTIFF4/tif_write.c", "src/Source/LibTIFF4/tif_zip.c", "src/Source/LibTIFF4/t4.h", "src/Source/LibTIFF4/tif_config.h", "src/Source/LibTIFF4/tif_dir.h", "src/Source/LibTIFF4/tif_fax3.h", "src/Source/LibTIFF4/tif_predict.h", "src/Source/LibTIFF4/tiff.h", "src/Source/LibTIFF4/tiffconf.h", "src/Source/LibTIFF4/tiffio.h", "src/Source/LibTIFF4/tiffiop.h", "src/Source/LibTIFF4/tiffvers.h", "src/Source/LibTIFF4/uvcode.h"
  }
