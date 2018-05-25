set (CMAKE_CXX_STANDARD 11)

set (SOURCE src/code/Assimp.cpp
  src/code/BaseImporter.h
  src/code/BaseImporter.cpp
  src/code/BaseProcess.h
  src/code/BaseProcess.cpp
  src/code/CalcTangentsProcess.cpp
  src/code/CalcTangentsProcess.h
  src/code/ConvertToLHProcess.h
  src/code/ConvertToLHProcess.cpp
  src/code/ComputeUVMappingProcess.cpp
  src/code/ComputeUVMappingProcess.h
  src/code/DeboneProcess.cpp
  src/code/DeboneProcess.h
  src/code/DefaultIOStream.h
  src/code/DefaultIOStream.cpp
  src/code/DefaultIOSystem.h
  src/code/DefaultIOSystem.cpp
  src/code/DefaultLogger.cpp
  src/code/FindDegenerates.cpp
  src/code/FindDegenerates.h
  src/code/FindInstancesProcess.cpp
  src/code/FindInstancesProcess.h
  src/code/FindInvalidDataProcess.cpp
  src/code/FindInvalidDataProcess.h
  src/code/FixNormalsStep.cpp
  src/code/FixNormalsStep.h
  src/code/GenFaceNormalsProcess.cpp
  src/code/GenFaceNormalsProcess.h
  src/code/GenVertexNormalsProcess.cpp
  src/code/GenVertexNormalsProcess.h
  src/code/Importer.h
  src/code/Importer.cpp
  src/code/ImporterRegistry.cpp
  src/code/ImproveCacheLocality.cpp
  src/code/ImproveCacheLocality.h
  src/code/JoinVerticesProcess.cpp
  src/code/JoinVerticesProcess.h
  src/code/LimitBoneWeightsProcess.cpp
  src/code/LimitBoneWeightsProcess.h
  src/code/MaterialSystem.h
  src/code/MaterialSystem.cpp
  src/code/OptimizeGraph.cpp
  src/code/OptimizeGraph.h
  src/code/OptimizeMeshes.cpp
  src/code/OptimizeMeshes.h
  src/code/PostStepRegistry.cpp
  src/code/PretransformVertices.cpp
  src/code/PretransformVertices.h
  src/code/ProcessHelper.h
  src/code/ProcessHelper.cpp
  src/code/RemoveVCProcess.cpp
  src/code/RemoveVCProcess.h
  src/code/RemoveRedundantMaterials.cpp
  src/code/RemoveRedundantMaterials.h
  src/code/SceneCombiner.h
  src/code/SceneCombiner.cpp
  src/code/ScenePreprocessor.h
  src/code/ScenePreprocessor.cpp
  src/code/SortByPTypeProcess.cpp
  src/code/SortByPTypeProcess.h
  src/code/SpatialSort.h
  src/code/SpatialSort.cpp
  src/code/SplitByBoneCountProcess.cpp
  src/code/SplitByBoneCountProcess.h
  src/code/SplitLargeMeshes.cpp
  src/code/SplitLargeMeshes.h
  src/code/TextureTransform.cpp
  src/code/TextureTransform.h
  src/code/TriangulateProcess.cpp
  src/code/TriangulateProcess.h
  src/code/ValidateDataStructure.h
  src/code/ValidateDataStructure.cpp
  src/code/Version.cpp
  src/code/VertexTriangleAdjacency.h
  src/code/VertexTriangleAdjacency.cpp

  src/code/XFileHelper.h
  src/code/XFileImporter.cpp
  src/code/XFileImporter.h
  src/code/XFileParser.cpp
  src/code/XFileParser.h
)

IF(NOT WIN32)
  set (SOURCE ${SOURCE} src/contrib/ConvertUTF/ConvertUTF.c src/contrib/ConvertUTF/ConvertUTF.h)
ENDIF()

add_library (assimp STATIC ${SOURCE})

target_include_directories(assimp SYSTEM PUBLIC src/include include)
target_link_libraries(assimp z)

target_compile_definitions(assimp PRIVATE
  -DASSIMP_BUILD_NO_OWN_ZLIB
  -DASSIMP_BUILD_NO_3DS_IMPORTER
  -DASSIMP_BUILD_NO_3DS_EXPORTER
  -DASSIMP_BUILD_NO_MD3_IMPORTER
  -DASSIMP_BUILD_NO_MDL_IMPORTER
  -DASSIMP_BUILD_NO_MD2_IMPORTER
  -DASSIMP_BUILD_NO_PLY_IMPORTER
  -DASSIMP_BUILD_NO_ASE_IMPORTER
  -DASSIMP_BUILD_NO_OBJ_IMPORTER
  -DASSIMP_BUILD_NO_HMP_IMPORTER
  -DASSIMP_BUILD_NO_SMD_IMPORTER
  -DASSIMP_BUILD_NO_MDC_IMPORTER
  -DASSIMP_BUILD_NO_MD5_IMPORTER
  -DASSIMP_BUILD_NO_STL_IMPORTER
  -DASSIMP_BUILD_NO_LWO_IMPORTER
  -DASSIMP_BUILD_NO_DXF_IMPORTER
  -DASSIMP_BUILD_NO_NFF_IMPORTER
  -DASSIMP_BUILD_NO_RAW_IMPORTER
  -DASSIMP_BUILD_NO_SIB_IMPORTER
  -DASSIMP_BUILD_NO_OFF_IMPORTER
  -DASSIMP_BUILD_NO_AC_IMPORTER
  -DASSIMP_BUILD_NO_BVH_IMPORTER
  -DASSIMP_BUILD_NO_IRRMESH_IMPORTER
  -DASSIMP_BUILD_NO_IRR_IMPORTER
  -DASSIMP_BUILD_NO_Q3D_IMPORTER
  -DASSIMP_BUILD_NO_B3D_IMPORTER
  -DASSIMP_BUILD_NO_COLLADA_IMPORTER
  -DASSIMP_BUILD_NO_TERRAGEN_IMPORTER
  -DASSIMP_BUILD_NO_CSM_IMPORTER
  -DASSIMP_BUILD_NO_3D_IMPORTER
  -DASSIMP_BUILD_NO_LWS_IMPORTER
  -DASSIMP_BUILD_NO_OGRE_IMPORTER
  -DASSIMP_BUILD_NO_OPENGEX_IMPORTER
  -DASSIMP_BUILD_NO_MS3D_IMPORTER
  -DASSIMP_BUILD_NO_COB_IMPORTER
  -DASSIMP_BUILD_NO_BLEND_IMPORTER
  -DASSIMP_BUILD_NO_Q3BSP_IMPORTER
  -DASSIMP_BUILD_NO_NDO_IMPORTER
  -DASSIMP_BUILD_NO_IFC_IMPORTER
  -DASSIMP_BUILD_NO_XGL_IMPORTER
  -DASSIMP_BUILD_NO_FBX_IMPORTER
  -DASSIMP_BUILD_NO_ASSBIN_IMPORTER
  -DASSIMP_BUILD_NO_GLTF_IMPORTER
  -DASSIMP_BUILD_NO_C4D_IMPORTER
  -DASSIMP_BUILD_NO_3MF_IMPORTER

  # TODO: once the ASSIMP loader code is solid we can disable a bunch of these
  # -DASSIMP_BUILD_NO_MAKELEFTHANDED_PROCESS
  # -DASSIMP_BUILD_NO_FLIPUVS_PROCESS
  # -DASSIMP_BUILD_NO_FLIPWINDINGORDER_PROCESS
  # -DASSIMP_BUILD_NO_REMOVEVC_PROCESS
  # -DASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS
  # -DASSIMP_BUILD_NO_FINDINSTANCES_PROCESS
  # -DASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS
  # -DASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS
  # -DASSIMP_BUILD_NO_GENUVCOORDS_PROCESS
  # -DASSIMP_BUILD_NO_TRANSFORMTEXCOORDS_PROCESS
  # -DASSIMP_BUILD_NO_PRETRANSFORMVERTICES_PROCESS
  # -DASSIMP_BUILD_NO_TRIANGULATE_PROCESS
  # -DASSIMP_BUILD_NO_SORTBYPTYPE_PROCESS
  # -DASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS
  # -DASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS
  # -DASSIMP_BUILD_NO_FIXINFACINGNORMALS_PROCESS
  # -DASSIMP_BUILD_NO_SPLITBYBONECOUNT_PROCESS
  # -DASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS
  # -DASSIMP_BUILD_NO_GENFACENORMALS_PROCESS
  # -DASSIMP_BUILD_NO_GENVERTEXNORMALS_PROCESS
  # -DASSIMP_BUILD_NO_CALCTANGENTS_PROCESS
  # -DASSIMP_BUILD_NO_JOINVERTICES_PROCESS
  # -DASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS
  # -DASSIMP_BUILD_NO_DEBONE_PROCESS
  # -DASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS
  # -DASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS
)
