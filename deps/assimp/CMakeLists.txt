# set(CMAKE_UNITY_BUILD ${ENABLE_UNITY_BUILD})

set (SOURCE tree/code/Assimp.cpp
  tree/code/BaseImporter.h
  tree/code/BaseImporter.cpp
  tree/code/BaseProcess.h
  tree/code/BaseProcess.cpp
  tree/code/CalcTangentsProcess.cpp
  tree/code/CalcTangentsProcess.h
  tree/code/ConvertToLHProcess.h
  tree/code/ConvertToLHProcess.cpp
  tree/code/ComputeUVMappingProcess.cpp
  tree/code/ComputeUVMappingProcess.h
  tree/code/DeboneProcess.cpp
  tree/code/DeboneProcess.h
  tree/code/DefaultIOStream.h
  tree/code/DefaultIOStream.cpp
  tree/code/DefaultIOSystem.h
  tree/code/DefaultIOSystem.cpp
  tree/code/DefaultLogger.cpp
  tree/code/FindDegenerates.cpp
  tree/code/FindDegenerates.h
  tree/code/FindInstancesProcess.cpp
  tree/code/FindInstancesProcess.h
  tree/code/FindInvalidDataProcess.cpp
  tree/code/FindInvalidDataProcess.h
  tree/code/FixNormalsStep.cpp
  tree/code/FixNormalsStep.h
  tree/code/GenFaceNormalsProcess.cpp
  tree/code/GenFaceNormalsProcess.h
  tree/code/GenVertexNormalsProcess.cpp
  tree/code/GenVertexNormalsProcess.h
  tree/code/Importer.h
  tree/code/Importer.cpp
  tree/code/ImporterRegistry.cpp
  tree/code/ImproveCacheLocality.cpp
  tree/code/ImproveCacheLocality.h
  tree/code/JoinVerticesProcess.cpp
  tree/code/JoinVerticesProcess.h
  tree/code/LimitBoneWeightsProcess.cpp
  tree/code/LimitBoneWeightsProcess.h
  tree/code/MaterialSystem.h
  tree/code/MaterialSystem.cpp
  tree/code/OptimizeGraph.cpp
  tree/code/OptimizeGraph.h
  tree/code/OptimizeMeshes.cpp
  tree/code/OptimizeMeshes.h
  tree/code/PostStepRegistry.cpp
  tree/code/PretransformVertices.cpp
  tree/code/PretransformVertices.h
  tree/code/ProcessHelper.h
  tree/code/ProcessHelper.cpp
  tree/code/RemoveVCProcess.cpp
  tree/code/RemoveVCProcess.h
  tree/code/RemoveRedundantMaterials.cpp
  tree/code/RemoveRedundantMaterials.h
  tree/code/SceneCombiner.h
  tree/code/SceneCombiner.cpp
  tree/code/ScenePreprocessor.h
  tree/code/ScenePreprocessor.cpp
  tree/code/SortByPTypeProcess.cpp
  tree/code/SortByPTypeProcess.h
  tree/code/SpatialSort.h
  tree/code/SpatialSort.cpp
  tree/code/SplitByBoneCountProcess.cpp
  tree/code/SplitByBoneCountProcess.h
  tree/code/SplitLargeMeshes.cpp
  tree/code/SplitLargeMeshes.h
  tree/code/TextureTransform.cpp
  tree/code/TextureTransform.h
  tree/code/TriangulateProcess.cpp
  tree/code/TriangulateProcess.h
  tree/code/ValidateDataStructure.h
  tree/code/ValidateDataStructure.cpp
  tree/code/Version.cpp
  tree/code/VertexTriangleAdjacency.h
  tree/code/VertexTriangleAdjacency.cpp

  tree/code/XFileHelper.h
  tree/code/XFileImporter.cpp
  tree/code/XFileImporter.h
  tree/code/XFileParser.cpp
  tree/code/XFileParser.h
)

set (SOURCE ${SOURCE} tree/contrib/ConvertUTF/ConvertUTF.c tree/contrib/ConvertUTF/ConvertUTF.h)

add_library (assimp STATIC ${SOURCE})
target_output_to_toolchain(assimp)

target_include_directories(assimp SYSTEM PUBLIC tree/include include)
target_link_libraries(assimp ${ZLIB})

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
