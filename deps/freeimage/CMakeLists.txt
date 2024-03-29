if(NOT BB_DESKTOP AND NOT BB_NDK AND NOT BB_NX)
  return()
endif()

set(CMAKE_CXX_STANDARD 14)

# JPEG
add_library(jpeg OBJECT src/Source/LibJPEG/jaricom.c src/Source/LibJPEG/jcapimin.c src/Source/LibJPEG/jcapistd.c src/Source/LibJPEG/jcarith.c src/Source/LibJPEG/jccoefct.c src/Source/LibJPEG/jccolor.c src/Source/LibJPEG/jcdctmgr.c src/Source/LibJPEG/jchuff.c src/Source/LibJPEG/jcinit.c src/Source/LibJPEG/jcmainct.c src/Source/LibJPEG/jcmarker.c src/Source/LibJPEG/jcmaster.c src/Source/LibJPEG/jcomapi.c src/Source/LibJPEG/jcparam.c src/Source/LibJPEG/jcprepct.c src/Source/LibJPEG/jcsample.c src/Source/LibJPEG/jctrans.c src/Source/LibJPEG/jdapimin.c src/Source/LibJPEG/jdapistd.c src/Source/LibJPEG/jdarith.c src/Source/LibJPEG/jdatadst.c src/Source/LibJPEG/jdatasrc.c src/Source/LibJPEG/jdcoefct.c src/Source/LibJPEG/jdcolor.c src/Source/LibJPEG/jddctmgr.c src/Source/LibJPEG/jdhuff.c src/Source/LibJPEG/jdinput.c src/Source/LibJPEG/jdmainct.c src/Source/LibJPEG/jdmarker.c src/Source/LibJPEG/jdmaster.c src/Source/LibJPEG/jdmerge.c src/Source/LibJPEG/jdpostct.c src/Source/LibJPEG/jdsample.c src/Source/LibJPEG/jdtrans.c src/Source/LibJPEG/jerror.c src/Source/LibJPEG/jfdctflt.c src/Source/LibJPEG/jfdctfst.c src/Source/LibJPEG/jfdctint.c src/Source/LibJPEG/jidctflt.c src/Source/LibJPEG/jidctfst.c src/Source/LibJPEG/jidctint.c src/Source/LibJPEG/jmemmgr.c src/Source/LibJPEG/jmemnobs.c src/Source/LibJPEG/jquant1.c src/Source/LibJPEG/jquant2.c src/Source/LibJPEG/jutils.c src/Source/LibJPEG/transupp.c)
target_include_directories(jpeg PUBLIC src/Source/LibJPEG)
set_target_properties(jpeg PROPERTIES UNITY_BUILD false)


# JXR
# IF(BB_MSVC)
#   add_library(jxr OBJECT src/Source/LibJXR/image/decode/decode.c src/Source/LibJXR/image/decode/JXRTranscode.c src/Source/LibJXR/image/decode/postprocess.c src/Source/LibJXR/image/decode/segdec.c src/Source/LibJXR/image/decode/strdec.c src/Source/LibJXR/image/decode/strdec_x86.c src/Source/LibJXR/image/decode/strInvTransform.c src/Source/LibJXR/image/decode/strPredQuantDec.c src/Source/LibJXR/image/encode/encode.c src/Source/LibJXR/image/encode/segenc.c src/Source/LibJXR/image/encode/strenc.c src/Source/LibJXR/image/encode/strenc_x86.c src/Source/LibJXR/image/encode/strFwdTransform.c src/Source/LibJXR/image/encode/strPredQuantEnc.c src/Source/LibJXR/image/sys/adapthuff.c src/Source/LibJXR/image/sys/image.c src/Source/LibJXR/image/sys/strcodec.c src/Source/LibJXR/image/sys/strPredQuant.c src/Source/LibJXR/image/sys/strTransform.c src/Source/LibJXR/jxrgluelib/JXRGlue.c src/Source/LibJXR/jxrgluelib/JXRGlueJxr.c src/Source/LibJXR/jxrgluelib/JXRGluePFC.c src/Source/LibJXR/jxrgluelib/JXRMeta.c src/Source/LibJXR/image/decode/decode.h src/Source/LibJXR/image/encode/encode.h src/Source/LibJXR/image/sys/ansi.h src/Source/LibJXR/image/sys/common.h src/Source/LibJXR/image/sys/perfTimer.h src/Source/LibJXR/image/sys/strcodec.h src/Source/LibJXR/image/sys/strTransform.h src/Source/LibJXR/image/sys/windowsmediaphoto.h src/Source/LibJXR/image/sys/xplatform_image.h src/Source/LibJXR/image/x86/x86.h src/Source/LibJXR/common/include/guiddef.h src/Source/LibJXR/common/include/wmsal.h src/Source/LibJXR/common/include/wmspecstring.h src/Source/LibJXR/common/include/wmspecstrings_adt.h src/Source/LibJXR/common/include/wmspecstrings_strict.h src/Source/LibJXR/common/include/wmspecstrings_undef.h src/Source/LibJXR/jxrgluelib/JXRGlue.h src/Source/LibJXR/jxrgluelib/JXRMeta.h)
#
#   target_compile_options(jxr PRIVATE -DDISABLE_PERF_MEASUREMENT)
#   target_include_directories(jxr PRIVATE src/Source/LibJXR/jxrgluelib src/Source/LibJXR/common/include src/Source/LibJXR/image/sys)
#   set_target_properties(jxr PROPERTIES UNITY_BUILD false)
#
#   IF(BB_LINUX)
#     target_compile_options(jxr PRIVATE -D__ANSI__)
#   ENDIF()
# ENDIF()

# openexr
# if(NOT BB_MINGW64)
#   add_library(openexr OBJECT src/Source/OpenEXR/IexMath/IexMathFpu.cpp src/Source/OpenEXR/IlmImf/ImfAcesFile.cpp src/Source/OpenEXR/IlmImf/ImfAttribute.cpp src/Source/OpenEXR/IlmImf/ImfB44Compressor.cpp src/Source/OpenEXR/IlmImf/ImfBoxAttribute.cpp src/Source/OpenEXR/IlmImf/ImfChannelList.cpp src/Source/OpenEXR/IlmImf/ImfChannelListAttribute.cpp src/Source/OpenEXR/IlmImf/ImfChromaticities.cpp src/Source/OpenEXR/IlmImf/ImfChromaticitiesAttribute.cpp src/Source/OpenEXR/IlmImf/ImfCompositeDeepScanLine.cpp src/Source/OpenEXR/IlmImf/ImfCompressionAttribute.cpp src/Source/OpenEXR/IlmImf/ImfCompressor.cpp src/Source/OpenEXR/IlmImf/ImfConvert.cpp src/Source/OpenEXR/IlmImf/ImfCRgbaFile.cpp src/Source/OpenEXR/IlmImf/ImfDeepCompositing.cpp src/Source/OpenEXR/IlmImf/ImfDeepFrameBuffer.cpp src/Source/OpenEXR/IlmImf/ImfDeepImageStateAttribute.cpp src/Source/OpenEXR/IlmImf/ImfDeepScanLineInputFile.cpp src/Source/OpenEXR/IlmImf/ImfDeepScanLineInputPart.cpp src/Source/OpenEXR/IlmImf/ImfDeepScanLineOutputFile.cpp src/Source/OpenEXR/IlmImf/ImfDeepScanLineOutputPart.cpp src/Source/OpenEXR/IlmImf/ImfDeepTiledInputFile.cpp src/Source/OpenEXR/IlmImf/ImfDeepTiledInputPart.cpp src/Source/OpenEXR/IlmImf/ImfDeepTiledOutputFile.cpp src/Source/OpenEXR/IlmImf/ImfDeepTiledOutputPart.cpp src/Source/OpenEXR/IlmImf/ImfDoubleAttribute.cpp src/Source/OpenEXR/IlmImf/ImfDwaCompressor.cpp src/Source/OpenEXR/IlmImf/ImfEnvmap.cpp src/Source/OpenEXR/IlmImf/ImfEnvmapAttribute.cpp src/Source/OpenEXR/IlmImf/ImfFastHuf.cpp src/Source/OpenEXR/IlmImf/ImfFloatAttribute.cpp src/Source/OpenEXR/IlmImf/ImfFloatVectorAttribute.cpp src/Source/OpenEXR/IlmImf/ImfFrameBuffer.cpp src/Source/OpenEXR/IlmImf/ImfFramesPerSecond.cpp src/Source/OpenEXR/IlmImf/ImfGenericInputFile.cpp src/Source/OpenEXR/IlmImf/ImfGenericOutputFile.cpp src/Source/OpenEXR/IlmImf/ImfHeader.cpp src/Source/OpenEXR/IlmImf/ImfHuf.cpp src/Source/OpenEXR/IlmImf/ImfInputFile.cpp src/Source/OpenEXR/IlmImf/ImfInputPart.cpp src/Source/OpenEXR/IlmImf/ImfInputPartData.cpp src/Source/OpenEXR/IlmImf/ImfIntAttribute.cpp src/Source/OpenEXR/IlmImf/ImfIO.cpp src/Source/OpenEXR/IlmImf/ImfKeyCode.cpp src/Source/OpenEXR/IlmImf/ImfKeyCodeAttribute.cpp src/Source/OpenEXR/IlmImf/ImfLineOrderAttribute.cpp src/Source/OpenEXR/IlmImf/ImfLut.cpp src/Source/OpenEXR/IlmImf/ImfMatrixAttribute.cpp src/Source/OpenEXR/IlmImf/ImfMisc.cpp src/Source/OpenEXR/IlmImf/ImfMultiPartInputFile.cpp src/Source/OpenEXR/IlmImf/ImfMultiPartOutputFile.cpp src/Source/OpenEXR/IlmImf/ImfMultiView.cpp src/Source/OpenEXR/IlmImf/ImfOpaqueAttribute.cpp src/Source/OpenEXR/IlmImf/ImfOutputFile.cpp src/Source/OpenEXR/IlmImf/ImfOutputPart.cpp src/Source/OpenEXR/IlmImf/ImfOutputPartData.cpp src/Source/OpenEXR/IlmImf/ImfPartType.cpp src/Source/OpenEXR/IlmImf/ImfPizCompressor.cpp src/Source/OpenEXR/IlmImf/ImfPreviewImage.cpp src/Source/OpenEXR/IlmImf/ImfPreviewImageAttribute.cpp src/Source/OpenEXR/IlmImf/ImfPxr24Compressor.cpp src/Source/OpenEXR/IlmImf/ImfRational.cpp src/Source/OpenEXR/IlmImf/ImfRationalAttribute.cpp src/Source/OpenEXR/IlmImf/ImfRgbaFile.cpp src/Source/OpenEXR/IlmImf/ImfRgbaYca.cpp src/Source/OpenEXR/IlmImf/ImfRle.cpp src/Source/OpenEXR/IlmImf/ImfRleCompressor.cpp src/Source/OpenEXR/IlmImf/ImfScanLineInputFile.cpp src/Source/OpenEXR/IlmImf/ImfStandardAttributes.cpp src/Source/OpenEXR/IlmImf/ImfStdIO.cpp src/Source/OpenEXR/IlmImf/ImfStringAttribute.cpp src/Source/OpenEXR/IlmImf/ImfStringVectorAttribute.cpp src/Source/OpenEXR/IlmImf/ImfSystemSpecific.cpp src/Source/OpenEXR/IlmImf/ImfTestFile.cpp src/Source/OpenEXR/IlmImf/ImfThreading.cpp src/Source/OpenEXR/IlmImf/ImfTileDescriptionAttribute.cpp src/Source/OpenEXR/IlmImf/ImfTiledInputFile.cpp src/Source/OpenEXR/IlmImf/ImfTiledInputPart.cpp src/Source/OpenEXR/IlmImf/ImfTiledMisc.cpp src/Source/OpenEXR/IlmImf/ImfTiledOutputFile.cpp src/Source/OpenEXR/IlmImf/ImfTiledOutputPart.cpp src/Source/OpenEXR/IlmImf/ImfTiledRgbaFile.cpp src/Source/OpenEXR/IlmImf/ImfTileOffsets.cpp src/Source/OpenEXR/IlmImf/ImfTimeCode.cpp src/Source/OpenEXR/IlmImf/ImfTimeCodeAttribute.cpp src/Source/OpenEXR/IlmImf/ImfVecAttribute.cpp src/Source/OpenEXR/IlmImf/ImfVersion.cpp src/Source/OpenEXR/IlmImf/ImfWav.cpp src/Source/OpenEXR/IlmImf/ImfZip.cpp src/Source/OpenEXR/IlmImf/ImfZipCompressor.cpp src/Source/OpenEXR/Imath/ImathBox.cpp src/Source/OpenEXR/Imath/ImathColorAlgo.cpp src/Source/OpenEXR/Imath/ImathFun.cpp src/Source/OpenEXR/Imath/ImathMatrixAlgo.cpp src/Source/OpenEXR/Imath/ImathRandom.cpp src/Source/OpenEXR/Imath/ImathShear.cpp src/Source/OpenEXR/Imath/ImathVec.cpp src/Source/OpenEXR/Iex/IexBaseExc.cpp src/Source/OpenEXR/Iex/IexThrowErrnoExc.cpp src/Source/OpenEXR/Half/half.cpp src/Source/OpenEXR/IlmThread/IlmThread.cpp src/Source/OpenEXR/IlmThread/IlmThreadMutex.cpp src/Source/OpenEXR/IlmThread/IlmThreadPool.cpp src/Source/OpenEXR/IlmThread/IlmThreadSemaphore.cpp)
#
#   target_include_directories(openexr PRIVATE
#     src/Source/OpenEXR/Half
#     src/Source/OpenEXR/Iex
#     src/Source/OpenEXR/IexMath
#     src/Source/OpenEXR/IlmImf
#     src/Source/OpenEXR/IlmThread
#     src/Source/OpenEXR/Imath
#     src/Source/OpenEXR
#   )
#
#   target_link_libraries(openexr ${ZLIB})
#   set_target_properties(openexr PROPERTIES UNITY_BUILD false)
#
# endif()

# openjpeg
# add_library(openjpeg OBJECT src/Source/LibOpenJPEG/bio.c src/Source/LibOpenJPEG/cio.c src/Source/LibOpenJPEG/dwt.c src/Source/LibOpenJPEG/event.c src/Source/LibOpenJPEG/function_list.c src/Source/LibOpenJPEG/image.c src/Source/LibOpenJPEG/invert.c src/Source/LibOpenJPEG/j2k.c src/Source/LibOpenJPEG/jp2.c src/Source/LibOpenJPEG/mct.c src/Source/LibOpenJPEG/mqc.c src/Source/LibOpenJPEG/openjpeg.c src/Source/LibOpenJPEG/opj_clock.c src/Source/LibOpenJPEG/pi.c src/Source/LibOpenJPEG/raw.c src/Source/LibOpenJPEG/t1.c src/Source/LibOpenJPEG/t2.c src/Source/LibOpenJPEG/tcd.c src/Source/LibOpenJPEG/tgt.c)
# target_compile_definitions(openjpeg PUBLIC -DOPJ_STATIC)
# set_target_properties(openjpeg PROPERTIES UNITY_BUILD false)

# raw
# add_library(raw_ OBJECT src/Source/LibRawLite/internal/dcraw_common.cpp src/Source/LibRawLite/internal/dcraw_fileio.cpp src/Source/LibRawLite/internal/demosaic_packs.cpp src/Source/LibRawLite/src/libraw_c_api.cpp src/Source/LibRawLite/src/libraw_cxx.cpp src/Source/LibRawLite/src/libraw_datastream.cpp src/Source/LibRawLite/internal/defines.h src/Source/LibRawLite/internal/libraw_internal_funcs.h src/Source/LibRawLite/internal/var_defines.h src/Source/LibRawLite/libraw/libraw.h src/Source/LibRawLite/libraw/libraw_alloc.h src/Source/LibRawLite/libraw/libraw_const.h src/Source/LibRawLite/libraw/libraw_datastream.h src/Source/LibRawLite/libraw/libraw_internal.h src/Source/LibRawLite/libraw/libraw_types.h src/Source/LibRawLite/libraw/libraw_version.h)
#
# target_include_directories(raw_ PRIVATE src/Source/LibRawLite)
#
# target_compile_definitions(raw_ PRIVATE -DLIBRAW_NODLL)
# IF(NOT BB_WIN32 AND NOT BB_WIN64)
#   target_compile_options(raw_ PRIVATE -Wno-narrowing)
# ENDIF()
# set_target_properties(raw_ PROPERTIES UNITY_BUILD false)

# tiff4
# add_library(tiff4 OBJECT src/Source/LibTIFF4/tif_aux.c src/Source/LibTIFF4/tif_close.c src/Source/LibTIFF4/tif_codec.c src/Source/LibTIFF4/tif_color.c src/Source/LibTIFF4/tif_compress.c src/Source/LibTIFF4/tif_dir.c src/Source/LibTIFF4/tif_dirinfo.c src/Source/LibTIFF4/tif_dirread.c src/Source/LibTIFF4/tif_dirwrite.c src/Source/LibTIFF4/tif_dumpmode.c src/Source/LibTIFF4/tif_error.c src/Source/LibTIFF4/tif_extension.c src/Source/LibTIFF4/tif_fax3.c src/Source/LibTIFF4/tif_fax3sm.c src/Source/LibTIFF4/tif_flush.c src/Source/LibTIFF4/tif_getimage.c src/Source/LibTIFF4/tif_jpeg.c src/Source/LibTIFF4/tif_luv.c src/Source/LibTIFF4/tif_lzma.c src/Source/LibTIFF4/tif_lzw.c src/Source/LibTIFF4/tif_next.c src/Source/LibTIFF4/tif_ojpeg.c src/Source/LibTIFF4/tif_open.c src/Source/LibTIFF4/tif_packbits.c src/Source/LibTIFF4/tif_pixarlog.c src/Source/LibTIFF4/tif_predict.c src/Source/LibTIFF4/tif_print.c src/Source/LibTIFF4/tif_read.c src/Source/LibTIFF4/tif_strip.c src/Source/LibTIFF4/tif_swab.c src/Source/LibTIFF4/tif_thunder.c src/Source/LibTIFF4/tif_tile.c src/Source/LibTIFF4/tif_version.c src/Source/LibTIFF4/tif_warning.c src/Source/LibTIFF4/tif_write.c src/Source/LibTIFF4/tif_zip.c)
#
# target_link_libraries(tiff4 jpeg ${ZLIB})
# target_compile_definitions(tiff4 PRIVATE -DTIFF_SSIZE_FORMAT=)

# webp
# add_library(webp OBJECT src/Source/LibWebP/src/dec/alpha_dec.c src/Source/LibWebP/src/dec/buffer_dec.c src/Source/LibWebP/src/dec/frame_dec.c src/Source/LibWebP/src/dec/idec_dec.c src/Source/LibWebP/src/dec/io_dec.c src/Source/LibWebP/src/dec/quant_dec.c src/Source/LibWebP/src/dec/tree_dec.c src/Source/LibWebP/src/dec/vp8l_dec.c src/Source/LibWebP/src/dec/vp8_dec.c src/Source/LibWebP/src/dec/webp_dec.c src/Source/LibWebP/src/demux/anim_decode.c src/Source/LibWebP/src/demux/demux.c src/Source/LibWebP/src/dsp/alpha_processing.c src/Source/LibWebP/src/dsp/alpha_processing_mips_dsp_r2.c src/Source/LibWebP/src/dsp/alpha_processing_neon.c src/Source/LibWebP/src/dsp/alpha_processing_sse2.c src/Source/LibWebP/src/dsp/alpha_processing_sse41.c src/Source/LibWebP/src/dsp/cost.c src/Source/LibWebP/src/dsp/cost_mips32.c src/Source/LibWebP/src/dsp/cost_mips_dsp_r2.c src/Source/LibWebP/src/dsp/cost_sse2.c src/Source/LibWebP/src/dsp/cpu.c src/Source/LibWebP/src/dsp/dec.c src/Source/LibWebP/src/dsp/dec_clip_tables.c src/Source/LibWebP/src/dsp/dec_mips32.c src/Source/LibWebP/src/dsp/dec_mips_dsp_r2.c src/Source/LibWebP/src/dsp/dec_msa.c src/Source/LibWebP/src/dsp/dec_neon.c src/Source/LibWebP/src/dsp/dec_sse2.c src/Source/LibWebP/src/dsp/dec_sse41.c src/Source/LibWebP/src/dsp/enc.c src/Source/LibWebP/src/dsp/enc_avx2.c src/Source/LibWebP/src/dsp/enc_mips32.c src/Source/LibWebP/src/dsp/enc_mips_dsp_r2.c src/Source/LibWebP/src/dsp/enc_msa.c src/Source/LibWebP/src/dsp/enc_neon.c src/Source/LibWebP/src/dsp/enc_sse2.c src/Source/LibWebP/src/dsp/enc_sse41.c src/Source/LibWebP/src/dsp/filters.c src/Source/LibWebP/src/dsp/filters_mips_dsp_r2.c src/Source/LibWebP/src/dsp/filters_msa.c src/Source/LibWebP/src/dsp/filters_neon.c src/Source/LibWebP/src/dsp/filters_sse2.c src/Source/LibWebP/src/dsp/lossless.c src/Source/LibWebP/src/dsp/lossless_enc.c src/Source/LibWebP/src/dsp/lossless_enc_mips32.c src/Source/LibWebP/src/dsp/lossless_enc_mips_dsp_r2.c src/Source/LibWebP/src/dsp/lossless_enc_msa.c src/Source/LibWebP/src/dsp/lossless_enc_neon.c src/Source/LibWebP/src/dsp/lossless_enc_sse2.c src/Source/LibWebP/src/dsp/lossless_enc_sse41.c src/Source/LibWebP/src/dsp/lossless_mips_dsp_r2.c src/Source/LibWebP/src/dsp/lossless_msa.c src/Source/LibWebP/src/dsp/lossless_neon.c src/Source/LibWebP/src/dsp/lossless_sse2.c src/Source/LibWebP/src/dsp/rescaler.c src/Source/LibWebP/src/dsp/rescaler_mips32.c src/Source/LibWebP/src/dsp/rescaler_mips_dsp_r2.c src/Source/LibWebP/src/dsp/rescaler_msa.c src/Source/LibWebP/src/dsp/rescaler_neon.c src/Source/LibWebP/src/dsp/rescaler_sse2.c src/Source/LibWebP/src/dsp/ssim.c src/Source/LibWebP/src/dsp/ssim_sse2.c src/Source/LibWebP/src/dsp/upsampling.c src/Source/LibWebP/src/dsp/upsampling_mips_dsp_r2.c src/Source/LibWebP/src/dsp/upsampling_msa.c src/Source/LibWebP/src/dsp/upsampling_neon.c src/Source/LibWebP/src/dsp/upsampling_sse2.c src/Source/LibWebP/src/dsp/upsampling_sse41.c src/Source/LibWebP/src/dsp/yuv.c src/Source/LibWebP/src/dsp/yuv_mips32.c src/Source/LibWebP/src/dsp/yuv_mips_dsp_r2.c src/Source/LibWebP/src/dsp/yuv_neon.c src/Source/LibWebP/src/dsp/yuv_sse2.c src/Source/LibWebP/src/dsp/yuv_sse41.c src/Source/LibWebP/src/enc/alpha_enc.c src/Source/LibWebP/src/enc/analysis_enc.c src/Source/LibWebP/src/enc/backward_references_cost_enc.c src/Source/LibWebP/src/enc/backward_references_enc.c src/Source/LibWebP/src/enc/config_enc.c src/Source/LibWebP/src/enc/cost_enc.c src/Source/LibWebP/src/enc/filter_enc.c src/Source/LibWebP/src/enc/frame_enc.c src/Source/LibWebP/src/enc/histogram_enc.c src/Source/LibWebP/src/enc/iterator_enc.c src/Source/LibWebP/src/enc/near_lossless_enc.c src/Source/LibWebP/src/enc/picture_csp_enc.c src/Source/LibWebP/src/enc/picture_enc.c src/Source/LibWebP/src/enc/picture_psnr_enc.c src/Source/LibWebP/src/enc/picture_rescale_enc.c src/Source/LibWebP/src/enc/picture_tools_enc.c src/Source/LibWebP/src/enc/predictor_enc.c src/Source/LibWebP/src/enc/quant_enc.c src/Source/LibWebP/src/enc/syntax_enc.c src/Source/LibWebP/src/enc/token_enc.c src/Source/LibWebP/src/enc/tree_enc.c src/Source/LibWebP/src/enc/vp8l_enc.c src/Source/LibWebP/src/enc/webp_enc.c src/Source/LibWebP/src/mux/anim_encode.c src/Source/LibWebP/src/mux/muxedit.c src/Source/LibWebP/src/mux/muxinternal.c src/Source/LibWebP/src/mux/muxread.c src/Source/LibWebP/src/utils/bit_reader_utils.c src/Source/LibWebP/src/utils/bit_writer_utils.c src/Source/LibWebP/src/utils/color_cache_utils.c src/Source/LibWebP/src/utils/filters_utils.c src/Source/LibWebP/src/utils/huffman_encode_utils.c src/Source/LibWebP/src/utils/huffman_utils.c src/Source/LibWebP/src/utils/quant_levels_dec_utils.c src/Source/LibWebP/src/utils/quant_levels_utils.c src/Source/LibWebP/src/utils/random_utils.c src/Source/LibWebP/src/utils/rescaler_utils.c src/Source/LibWebP/src/utils/thread_utils.c src/Source/LibWebP/src/utils/utils.c)
# target_include_directories(webp PUBLIC src/Source/LibWebP)
# set_target_properties(webp PROPERTIES UNITY_BUILD false)

# FreeImage
set(SOURCES
  src/Source/FreeImage/BitmapAccess.cpp
  # src/Source/FreeImage/ColorLookup.cpp
  # src/Source/FreeImage/ConversionRGBA16.cpp
  # src/Source/FreeImage/ConversionRGBAF.cpp
  src/Source/FreeImage/FreeImage.cpp
  # src/Source/FreeImage/FreeImageC.c
  src/Source/FreeImage/FreeImageIO.cpp
  src/Source/FreeImage/GetType.cpp
  src/Source/FreeImage/LFPQuantizer.cpp
  src/Source/FreeImage/MemoryIO.cpp
  src/Source/FreeImage/PixelAccess.cpp
  # src/Source/FreeImage/J2KHelper.cpp
  # src/Source/FreeImage/MNGHelper.cpp
  src/Source/FreeImage/Plugin.cpp
  src/Source/FreeImage/PluginBMP.cpp
  # src/Source/FreeImage/PluginCUT.cpp
  src/Source/FreeImage/PluginDDS.cpp
  # src/Source/FreeImage/PluginEXR.cpp
  # src/Source/FreeImage/PluginG3.cpp
  # src/Source/FreeImage/PluginGIF.cpp
  # src/Source/FreeImage/PluginHDR.cpp
  # src/Source/FreeImage/PluginICO.cpp
  src/Source/FreeImage/PluginIFF.cpp
  # src/Source/FreeImage/PluginJ2K.cpp
  # src/Source/FreeImage/PluginJNG.cpp
  # src/Source/FreeImage/PluginJP2.cpp
  src/Source/FreeImage/PluginJPEG.cpp
  # src/Source/FreeImage/PluginJXR.cpp
  # src/Source/FreeImage/PluginKOALA.cpp
  # src/Source/FreeImage/PluginMNG.cpp
  # src/Source/FreeImage/PluginPCD.cpp
  src/Source/FreeImage/PluginPCX.cpp
  # src/Source/FreeImage/PluginPFM.cpp
  # src/Source/FreeImage/PluginPICT.cpp
  src/Source/FreeImage/PluginPNG.cpp
  # src/Source/FreeImage/PluginPNM.cpp
  # src/Source/FreeImage/PluginPSD.cpp
  # src/Source/FreeImage/PluginRAS.cpp
  # src/Source/FreeImage/PluginRAW.cpp
  # src/Source/FreeImage/PluginSGI.cpp
  src/Source/FreeImage/PluginTARGA.cpp
  # src/Source/FreeImage/PluginTIFF.cpp
  # src/Source/FreeImage/PluginWBMP.cpp
  # src/Source/FreeImage/PluginWebP.cpp
  # src/Source/FreeImage/PluginXBM.cpp
  # src/Source/FreeImage/PluginXPM.cpp
  # src/Source/FreeImage/PSDParser.cpp
  # src/Source/FreeImage/TIFFLogLuv.cpp
  src/Source/FreeImage/Conversion.cpp
  src/Source/FreeImage/Conversion16_555.cpp
  src/Source/FreeImage/Conversion16_565.cpp
  src/Source/FreeImage/Conversion24.cpp
  src/Source/FreeImage/Conversion32.cpp
  # src/Source/FreeImage/Conversion4.cpp
  src/Source/FreeImage/Conversion8.cpp
  # src/Source/FreeImage/ConversionFloat.cpp
  src/Source/FreeImage/ConversionRGB16.cpp
  src/Source/FreeImage/ConversionRGBF.cpp
  # src/Source/FreeImage/ConversionType.cpp
  # src/Source/FreeImage/ConversionUINT16.cpp
  # src/Source/FreeImage/Halftoning.cpp
  # src/Source/FreeImage/tmoColorConvert.cpp
  # src/Source/FreeImage/tmoDrago03.cpp
  # src/Source/FreeImage/tmoFattal02.cpp
  # src/Source/FreeImage/tmoReinhard05.cpp
  # src/Source/FreeImage/ToneMapping.cpp
  src/Source/FreeImage/NNQuantizer.cpp
  src/Source/FreeImage/WuQuantizer.cpp
  # src/Source/FreeImage/CacheFile.cpp
  # src/Source/FreeImage/MultiPage.cpp
  # src/Source/FreeImage/ZLibInterface.cpp
  src/Source/Metadata/Exif.cpp
  # src/Source/Metadata/FIRational.cpp
  src/Source/Metadata/FreeImageTag.cpp
  src/Source/Metadata/IPTC.cpp
  # src/Source/Metadata/TagConversion.cpp
  src/Source/Metadata/TagLib.cpp
  # src/Source/Metadata/XTIFF.cpp
  # src/Source/FreeImageToolkit/Background.cpp
  # src/Source/FreeImageToolkit/BSplineRotate.cpp
  # src/Source/FreeImageToolkit/Channels.cpp
  src/Source/FreeImageToolkit/ClassicRotate.cpp
  # src/Source/FreeImageToolkit/Colors.cpp
  # src/Source/FreeImageToolkit/CopyPaste.cpp
  # src/Source/FreeImageToolkit/Display.cpp
  src/Source/FreeImageToolkit/Flip.cpp
  # src/Source/FreeImageToolkit/JPEGTransform.cpp
  # src/Source/FreeImageToolkit/MultigridPoissonSolver.cpp
  # src/Source/FreeImageToolkit/Rescale.cpp
  # src/Source/FreeImageToolkit/Resize.cpp

  # src/Source/OpenEXR/Half/half.cpp
)

set(DEPS jpeg png_static)

add_library(freeimage STATIC ${SOURCES})
target_output_to_toolchain(freeimage)
target_compile_definitions(freeimage PRIVATE -DLIBRAW_NODLL)
target_compile_definitions(freeimage PUBLIC -DFREEIMAGE_LIB)
target_include_directories(freeimage PUBLIC src/Source)

target_link_libraries(freeimage ${DEPS} png_static)
set_target_properties(freeimage PROPERTIES UNITY_BUILD false)
