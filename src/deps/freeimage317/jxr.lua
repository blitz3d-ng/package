project "jxr"
  removeplatforms { "macos" }
  kind "StaticLib"
  language "C"

  includedirs {
    "src/Source/LibJXR/jxrgluelib",
    "src/Source/LibJXR/common/include",
    "src/Source/LibJXR/image/sys"
  }

  defines "DISABLE_PERF_MEASUREMENT"

  files {
    "src/Source/LibJXR/image/decode/decode.c", "src/Source/LibJXR/image/decode/JXRTranscode.c", "src/Source/LibJXR/image/decode/postprocess.c", "src/Source/LibJXR/image/decode/segdec.c", "src/Source/LibJXR/image/decode/strdec.c", "src/Source/LibJXR/image/decode/strdec_x86.c", "src/Source/LibJXR/image/decode/strInvTransform.c", "src/Source/LibJXR/image/decode/strPredQuantDec.c", "src/Source/LibJXR/image/encode/encode.c", "src/Source/LibJXR/image/encode/segenc.c", "src/Source/LibJXR/image/encode/strenc.c", "src/Source/LibJXR/image/encode/strenc_x86.c", "src/Source/LibJXR/image/encode/strFwdTransform.c", "src/Source/LibJXR/image/encode/strPredQuantEnc.c", "src/Source/LibJXR/image/sys/adapthuff.c", "src/Source/LibJXR/image/sys/image.c", "src/Source/LibJXR/image/sys/strcodec.c", "src/Source/LibJXR/image/sys/strPredQuant.c", "src/Source/LibJXR/image/sys/strTransform.c", "src/Source/LibJXR/jxrgluelib/JXRGlue.c", "src/Source/LibJXR/jxrgluelib/JXRGlueJxr.c", "src/Source/LibJXR/jxrgluelib/JXRGluePFC.c", "src/Source/LibJXR/jxrgluelib/JXRMeta.c", "src/Source/LibJXR/image/decode/decode.h", "src/Source/LibJXR/image/encode/encode.h", "src/Source/LibJXR/image/sys/ansi.h", "src/Source/LibJXR/image/sys/common.h", "src/Source/LibJXR/image/sys/perfTimer.h", "src/Source/LibJXR/image/sys/strcodec.h", "src/Source/LibJXR/image/sys/strTransform.h", "src/Source/LibJXR/image/sys/windowsmediaphoto.h", "src/Source/LibJXR/image/sys/xplatform_image.h", "src/Source/LibJXR/image/x86/x86.h", "src/Source/LibJXR/common/include/guiddef.h", "src/Source/LibJXR/common/include/wmsal.h", "src/Source/LibJXR/common/include/wmspecstring.h", "src/Source/LibJXR/common/include/wmspecstrings_adt.h", "src/Source/LibJXR/common/include/wmspecstrings_strict.h", "src/Source/LibJXR/common/include/wmspecstrings_undef.h", "src/Source/LibJXR/jxrgluelib/JXRGlue.h", "src/Source/LibJXR/jxrgluelib/JXRMeta.h"
  }

  filter "platforms:linux"
    defines { "__ANSI__" }
