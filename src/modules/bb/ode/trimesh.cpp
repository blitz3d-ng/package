
#include "ode.h"
#include <ode/ode.h>

bb_ptr_t BBCALL _odeGeomTriMeshDataCreate(){
  return (bb_ptr_t)dGeomTriMeshDataCreate();
}

// ODE_API void dGeomTriMeshDataDestroy(dTriMeshDataID g);
// ODE_API void dGeomTriMeshDataSet(dTriMeshDataID g, int data_id, void* in_data);
// ODE_API void* dGeomTriMeshDataGet(dTriMeshDataID g, int data_id);
// ODE_API void dGeomTriMeshSetLastTransform( dGeomID g, dMatrix4 last_trans );
// ODE_API dReal* dGeomTriMeshGetLastTransform( dGeomID g );

// ODE_API void dGeomTriMeshDataBuildSingle(dTriMeshDataID g, const void* Vertices, int VertexStride, int VertexCount, const void* Indices, int IndexCount, int TriStride);

// ODE_API void dGeomTriMeshDataBuildSingle1(dTriMeshDataID g, const void* Vertices, int VertexStride, int VertexCount, const void* Indices, int IndexCount, int TriStride, const void* Normals);
// ODE_API void dGeomTriMeshDataBuildDouble(dTriMeshDataID g, const void* Vertices,  int VertexStride, int VertexCount, const void* Indices, int IndexCount, int TriStride);
// ODE_API void dGeomTriMeshDataBuildDouble1(dTriMeshDataID g, const void* Vertices,  int VertexStride, int VertexCount, const void* Indices, int IndexCount, int TriStride, const void* Normals);
// ODE_API void dGeomTriMeshDataBuildSimple(dTriMeshDataID g, const dReal* Vertices, int VertexCount, const dTriIndex* Indices, int IndexCount);
// ODE_API void dGeomTriMeshDataBuildSimple1(dTriMeshDataID g, const dReal* Vertices, int VertexCount, const dTriIndex* Indices, int IndexCount, const int* Normals);
// ODE_API void dGeomTriMeshDataPreprocess(dTriMeshDataID g);
// ODE_API void dGeomTriMeshDataGetBuffer(dTriMeshDataID g, unsigned char** buf, int* bufLen);
// ODE_API void dGeomTriMeshDataSetBuffer(dTriMeshDataID g, unsigned char* buf);
// ODE_API void dGeomTriMeshSetCallback(dGeomID g, dTriCallback* Callback);
// ODE_API dTriCallback* dGeomTriMeshGetCallback(dGeomID g);
// ODE_API void dGeomTriMeshSetArrayCallback(dGeomID g, dTriArrayCallback* ArrayCallback);
// ODE_API dTriArrayCallback* dGeomTriMeshGetArrayCallback(dGeomID g);
// ODE_API void dGeomTriMeshSetRayCallback(dGeomID g, dTriRayCallback* Callback);
// ODE_API dTriRayCallback* dGeomTriMeshGetRayCallback(dGeomID g);
// ODE_API void dGeomTriMeshSetTriMergeCallback(dGeomID g, dTriTriMergeCallback* Callback);
// ODE_API dTriTriMergeCallback* dGeomTriMeshGetTriMergeCallback(dGeomID g);
// ODE_API dGeomID dCreateTriMesh(dSpaceID space, dTriMeshDataID Data, dTriCallback* Callback, dTriArrayCallback* ArrayCallback, dTriRayCallback* RayCallback);
// ODE_API void dGeomTriMeshSetData(dGeomID g, dTriMeshDataID Data);
// ODE_API dTriMeshDataID dGeomTriMeshGetData(dGeomID g);
// ODE_API void dGeomTriMeshEnableTC(dGeomID g, int geomClass, int enable);
// ODE_API int dGeomTriMeshIsTCEnabled(dGeomID g, int geomClass);
// ODE_API void dGeomTriMeshClearTCCache(dGeomID g);
// ODE_API dTriMeshDataID dGeomTriMeshGetTriMeshDataID(dGeomID g);
// ODE_API void dGeomTriMeshGetTriangle(dGeomID g, int Index, dVector3* v0, dVector3* v1, dVector3* v2);
// ODE_API void dGeomTriMeshGetPoint(dGeomID g, int Index, dReal u, dReal v, dVector3 Out);
// ODE_API int dGeomTriMeshGetTriangleCount (dGeomID g);
// ODE_API void dGeomTriMeshDataUpdate(dTriMeshDataID g);
