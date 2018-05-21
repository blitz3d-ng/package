#ifndef BB_BLITZ3D_COMMANDS_H
#define BB_BLITZ3D_COMMANDS_H

#include <bb/audio/commands.h>
#include <bb/graphics/commands.h>

#ifdef __cplusplus
extern "C" {
#else
typedef void Texture;
typedef void Brush;
typedef void Entity;
typedef void MeshModel;
typedef void Surface;
typedef void Model;
typedef void Camera;
typedef void Object;
typedef void Light;
typedef void Sprite;
typedef void MD2Model;
typedef void Q3BSPModel;
typedef void Terrain;
#endif

///////////////////////////
// GLOBAL WORLD COMMANDS //
///////////////////////////
void BBCALL bbLoaderMatrix( BBStr *ext,float xx,float xy,float xz,float yx,float yy,float yz,float zx,float zy,float zz );
int  BBCALL bbHWTexUnits();
int	 BBCALL bbGfxDriverCaps3D();
void BBCALL bbHWMultiTex( int enable );
void BBCALL bbWBuffer( int enable );
void BBCALL bbDither( int enable );
void BBCALL bbAntiAlias( int enable );
void BBCALL bbWireFrame( int enable );
void BBCALL bbAmbientLight( float r,float g,float b );
void BBCALL bbClearCollisions();
void BBCALL bbCollisions( int src_type,int dest_type,int method,int response );
void BBCALL bbUpdateWorld( float elapsed );
void BBCALL bbCaptureWorld();
void BBCALL bbRenderWorld( float tween );
int BBCALL bbTrisRendered();
float BBCALL bbStats3D( int n );

//////////////////////
// TEXTURE COMMANDS //
//////////////////////

Texture * BBCALL bbLoadTexture( BBStr *file,int flags );
Texture * BBCALL bbLoadAnimTexture( BBStr *file,int flags,int w,int h,int first,int cnt );
Texture * BBCALL bbCreateTexture( int w,int h,int flags,int frames );
void BBCALL bbFreeTexture( Texture *t );
void BBCALL bbTextureBlend( Texture *t,int blend );
void BBCALL bbTextureCoords( Texture *t,int flags );
void BBCALL bbScaleTexture( Texture *t,float u_scale,float v_scale );
void BBCALL bbRotateTexture( Texture *t,float angle );
void BBCALL bbPositionTexture( Texture *t,float u_pos,float v_pos );
int BBCALL bbTextureWidth( Texture *t );
int BBCALL bbTextureHeight( Texture *t );
BBStr * BBCALL bbTextureName( Texture *t );
void BBCALL bbSetCubeFace( Texture *t,int face );
void BBCALL bbSetCubeMode( Texture *t,int mode );
BBCanvas * BBCALL bbTextureBuffer( Texture *t,int frame );
void BBCALL bbClearTextureFilters();
void BBCALL bbTextureFilter( BBStr *t,int flags );

////////////////////
// BRUSH COMMANDS //
////////////////////
Brush * BBCALL bbCreateBrush( float r,float g,float b );
Brush * BBCALL bbLoadBrush( BBStr *file,int flags,float u_scale,float v_scale );
void BBCALL bbFreeBrush( Brush *b );
void BBCALL bbBrushColor( Brush *br,float r,float g,float b );
int BBCALL bbGetBrushRed( Brush *b );
int BBCALL bbGetBrushGreen( Brush *b );
int BBCALL bbGetBrushBlue( Brush *b );
void BBCALL bbBrushAlpha( Brush *b,float alpha );
float BBCALL bbGetBrushAlpha( Brush *b );
void BBCALL bbBrushShininess( Brush *b,float n );
float BBCALL bbGetBrushShininess( Brush *b );
void BBCALL bbBrushTexture( Brush *b,Texture *t,int frame,int index );
Texture *bbGetBrushTexture( Brush *b,int index );
void BBCALL bbBrushBlend( Brush *b,int blend );
int BBCALL bbGetBrushBlend( Brush *b );
void BBCALL bbBrushFX( Brush *b,int fx );
int BBCALL bbGetBrushFX( Brush *b );

///////////////////
// MESH COMMANDS //
///////////////////
Entity * BBCALL bbCreateMesh( Entity *p );
Entity * BBCALL bbLoadMesh( BBStr *f,Entity *p );
Entity * BBCALL bbLoadAnimMesh( BBStr *f,Entity *p );
Entity * BBCALL bbCreateCube( Entity *p );
Entity * BBCALL bbCreateSphere( int segs,Entity *p );
Entity * BBCALL bbCreateCylinder( int segs,int solid,Entity *p );
Entity * BBCALL bbCreateCone( int segs,int solid,Entity *p );
Entity * BBCALL bbCopyMesh( MeshModel *m,Entity *p );
void BBCALL bbScaleMesh( MeshModel *m,float x,float y,float z );
void BBCALL bbRotateMesh( MeshModel *m,float x,float y,float z );
void BBCALL bbPositionMesh( MeshModel *m,float x,float y,float z );
void BBCALL bbFitMesh( MeshModel *m,float x,float y,float z,float w,float h,float d,int uniform );
void BBCALL bbFlipMesh( MeshModel *m );
void BBCALL bbPaintMesh( MeshModel *m,Brush *b );
void BBCALL bbAddMesh( MeshModel *src,MeshModel *dest );
void BBCALL bbUpdateNormals( MeshModel *m );
void BBCALL bbLightMesh( MeshModel *m,float r,float g,float b,float range,float x,float y,float z );
float BBCALL bbMeshWidth( MeshModel *m );
float BBCALL bbMeshHeight( MeshModel *m );
float BBCALL bbMeshDepth( MeshModel *m );
int BBCALL bbMeshesIntersect( MeshModel *a,MeshModel *b );
int BBCALL bbCountSurfaces( MeshModel *m );
Surface * BBCALL bbGetSurface( MeshModel *m,int index );
void BBCALL bbMeshCullBox( MeshModel *m,float x,float y,float z,float width,float height,float depth );


//////////////////////
// SURFACE COMMANDS //
//////////////////////
Surface * BBCALL bbFindSurface( MeshModel *m,Brush *b );
Surface * BBCALL bbCreateSurface( MeshModel *m,Brush *b );
Brush * BBCALL bbGetSurfaceBrush( Surface *s );
Brush * BBCALL bbGetEntityBrush( Model *m );
void BBCALL bbClearSurface( Surface *s,int verts,int tris );
void BBCALL bbPaintSurface( Surface *s,Brush *b );
int BBCALL bbAddVertex( Surface *s,float x,float y,float z,float tu,float tv,float tw );
int BBCALL bbAddTriangle( Surface *s,int v0,int v1,int v2 );
void BBCALL bbVertexCoords( Surface *s,int n,float x,float y,float z );
void BBCALL bbVertexNormal( Surface *s,int n,float x,float y,float z );
void BBCALL bbVertexColor( Surface *s,int n,float r,float g,float b,float a );
void BBCALL bbVertexTexCoords( Surface *s,int n,float u,float v,float w,int set );
int BBCALL bbCountVertices( Surface *s );
int BBCALL bbCountTriangles( Surface *s );
float BBCALL bbVertexX( Surface *s,int n );
float BBCALL bbVertexY( Surface *s,int n );
float BBCALL bbVertexZ( Surface *s,int n );
float BBCALL bbVertexNX( Surface *s,int n );
float BBCALL bbVertexNY( Surface *s,int n );
float BBCALL bbVertexNZ( Surface *s,int n );
float BBCALL bbVertexRed( Surface *s,int n );
float BBCALL bbVertexGreen( Surface *s,int n );
float BBCALL bbVertexBlue( Surface *s,int n );
float BBCALL bbVertexAlpha( Surface *s,int n );
float BBCALL bbVertexU( Surface *s,int n,int t );
float BBCALL bbVertexV( Surface *s,int n,int t );
float BBCALL bbVertexW( Surface *s,int n,int t );
int BBCALL bbTriangleVertex( Surface *s,int n,int v );

/////////////////////
// CAMERA COMMANDS //
/////////////////////
Entity * BBCALL bbCreateCamera( Entity *p );
void BBCALL bbCameraZoom( Camera *c,float zoom );
void BBCALL bbCameraRange( Camera *c,float nr,float fr );
void BBCALL bbCameraClsColor( Camera *c,float r,float g,float b );
void BBCALL bbCameraClsMode( Camera *c,int cls_color,int cls_zbuffer );
void BBCALL bbCameraProjMode( Camera *c,int mode );
void BBCALL bbCameraViewport( Camera *c,int x,int y,int w,int h );
void BBCALL bbCameraFogRange( Camera *c,float nr,float fr );
void BBCALL bbCameraFogColor( Camera *c,float r,float g,float b );
void BBCALL bbCameraFogMode( Camera *c,int mode );
int BBCALL bbCameraProject( Camera *c,float x,float y,float z );
float BBCALL bbProjectedX();
float BBCALL bbProjectedY();
float BBCALL bbProjectedZ();
Entity * BBCALL bbCameraPick( Camera *c,float x,float y );
Entity * BBCALL bbLinePick( float x,float y,float z,float dx,float dy,float dz,float radius );
Entity * BBCALL bbEntityPick( Object *src,float range );
int BBCALL bbEntityVisible( Object *src,Object *dest );
int BBCALL bbEntityInView( Entity *e,Camera *c );
float BBCALL bbPickedX();
float BBCALL bbPickedY();
float BBCALL bbPickedZ();
float BBCALL bbPickedNX();
float BBCALL bbPickedNY();
float BBCALL bbPickedNZ();
float BBCALL bbPickedTime();
Object * BBCALL bbPickedEntity();
void * BBCALL bbPickedSurface();
int BBCALL bbPickedTriangle();

////////////////////
// LIGHT COMMANDS //
////////////////////
Entity * BBCALL bbCreateLight( int type,Entity *p );
void BBCALL bbLightColor( Light *light,float r,float g,float b );
void BBCALL bbLightRange( Light *light,float range );
void BBCALL bbLightConeAngles( Light *light,float inner,float outer );

////////////////////
// PIVOT COMMANDS //
////////////////////
Entity * BBCALL bbCreatePivot( Entity *p );

/////////////////////
// SPRITE COMMANDS //
/////////////////////
Entity * BBCALL bbCreateSprite( Entity *p );
Entity * BBCALL bbLoadSprite( BBStr *file,int flags,Entity *p );
void BBCALL bbRotateSprite( Sprite *s,float angle );
void BBCALL bbScaleSprite( Sprite *s,float x,float y );
void BBCALL bbHandleSprite( Sprite *s,float x,float y );
void BBCALL bbSpriteViewMode( Sprite *s,int mode );

/////////////////////
// MIRROR COMMANDS //
/////////////////////
Entity * BBCALL bbCreateMirror( Entity *p );

////////////////////
// PLANE COMMANDS //
////////////////////
Entity * BBCALL bbCreatePlane( int segs,Entity *p );

//////////////////
// MD2 COMMANDS //
//////////////////
Entity * BBCALL bbLoadMD2( BBStr *file,Entity *p );
void BBCALL bbAnimateMD2( MD2Model *m,int mode,float speed,int first,int last,float trans );
float BBCALL bbMD2AnimTime( MD2Model *m );
int BBCALL bbMD2AnimLength( MD2Model *m );
int BBCALL bbMD2Animating( MD2Model *m );

//////////////////
// BSP Commands //
//////////////////
Entity * BBCALL bbLoadBSP( BBStr *file,float gam,Entity *p );
void BBCALL bbBSPAmbientLight( Q3BSPModel *t,float r,float g,float b );
void BBCALL bbBSPLighting( Q3BSPModel *t,int lmap );

//////////////////////
// TERRAIN COMMANDS //
//////////////////////
Entity * BBCALL bbCreateTerrain( int n,Entity *p );
Entity * BBCALL bbLoadTerrain( BBStr *file,Entity *p );
void BBCALL bbTerrainDetail( Terrain *t,int n,int m );
void BBCALL bbTerrainShading( Terrain *t,int enable );
float BBCALL bbTerrainX( Terrain *t,float x,float y,float z );
float BBCALL bbTerrainY( Terrain *t,float x,float y,float z );
float BBCALL bbTerrainZ( Terrain *t,float x,float y,float z );
int BBCALL bbTerrainSize( Terrain *t );
float BBCALL bbTerrainHeight( Terrain *t,int x,int z );
void BBCALL bbModifyTerrain( Terrain *t,int x,int z,float h,int realtime );

////////////////////
// AUDIO COMMANDS //
////////////////////
Entity * BBCALL bbCreateListener( Entity *p,float roll,float dopp,float dist );
BBChannel * BBCALL bbEmitSound( BBSound *sound,Object *o );

/////////////////////
// ENTITY COMMANDS //
/////////////////////
Entity * BBCALL bbCopyEntity( Entity *e,Entity *p );
void BBCALL bbFreeEntity( Entity *e );
void BBCALL bbHideEntity( Entity *e );
void BBCALL bbShowEntity( Entity *e );
void BBCALL bbEntityParent( Entity *e,Entity *p,int global );
int BBCALL bbCountChildren( Entity *e );
Entity * BBCALL bbGetChild( Entity *e,int index );
Entity * BBCALL bbFindChild( Entity *e,BBStr *t );

////////////////////////
// ANIMATION COMMANDS //
////////////////////////
int BBCALL bbLoadAnimSeq( Object *o,BBStr *f );
void BBCALL bbSetAnimTime( Object *o,float time,int seq );
void BBCALL bbAnimate( Object *o,int mode,float speed,int seq,float trans );
void BBCALL bbSetAnimKey( Object *o,int frame,int pos_key,int rot_key,int scl_key );
int BBCALL bbExtractAnimSeq( Object *o,int first,int last,int seq );
int BBCALL bbAddAnimSeq( Object *o,int length );
int BBCALL bbAnimSeq( Object *o );
float BBCALL bbAnimTime( Object *o );
int BBCALL bbAnimLength( Object *o );
int BBCALL bbAnimating( Object *o );

////////////////////////////////
// ENTITY SPECIAL FX COMMANDS //
////////////////////////////////
void BBCALL bbPaintEntity( Model *m,Brush *b );
void BBCALL bbEntityColor( Model *m,float r,float g,float b );
void BBCALL bbEntityAlpha( Model *m,float alpha );
void BBCALL bbEntityShininess( Model *m,float shininess );
void BBCALL bbEntityTexture( Model *m,Texture *t,int frame,int index );
void BBCALL bbEntityBlend( Model *m,int blend );
void BBCALL bbEntityFX( Model *m,int fx );
void BBCALL bbEntityAutoFade( Model *m,float nr,float fr );
void BBCALL bbEntityOrder( Object *o,int n );

//////////////////////////////
// ENTITY PROPERTY COMMANDS //
//////////////////////////////
float BBCALL bbEntityX( Entity *e,int global );
float BBCALL bbEntityY( Entity *e,int global );
float BBCALL bbEntityZ( Entity *e,int global );
float BBCALL bbEntityPitch( Entity *e,int global );
float BBCALL bbEntityYaw( Entity *e,int global );
float BBCALL bbEntityRoll( Entity *e,int global );
float BBCALL bbGetMatElement( Entity *e,int row,int col );

void BBCALL bbTFormPoint( float x,float y,float z,Entity *src,Entity *dest );
void BBCALL bbTFormVector( float x,float y,float z,Entity *src,Entity *dest );
void BBCALL bbTFormNormal( float x,float y,float z,Entity *src,Entity *dest );
float BBCALL bbTFormedX();
float BBCALL bbTFormedY();
float BBCALL bbTFormedZ();
float BBCALL bbVectorYaw( float x,float y,float z );
float BBCALL bbVectorPitch( float x,float y,float z );
float BBCALL bbDeltaYaw( Entity *src,Entity *dest );
float BBCALL bbDeltaPitch( Entity *src,Entity *dest );

///////////////////////////////
// ENTITY COLLISION COMMANDS //
///////////////////////////////
void BBCALL bbResetEntity( Object *o );
void BBCALL bbEntityType( Object *o,int type,int recurs );
void BBCALL bbEntityPickMode( Object *o,int mode,int obs );
Entity * BBCALL bbGetParent( Entity *e );
int BBCALL bbGetEntityType( Object *o );
void BBCALL bbEntityRadius( Object *o,float x_radius,float y_radius );
void BBCALL bbEntityBox( Object *o,float x,float y,float z,float w,float h,float d );
Object * BBCALL bbEntityCollided( Object *o,int type );
int BBCALL bbCountCollisions( Object *o );
float BBCALL bbCollisionX( Object *o,int index );
float BBCALL bbCollisionY( Object *o,int index );
float BBCALL bbCollisionZ( Object *o,int index );
float BBCALL bbCollisionNX( Object *o,int index );
float BBCALL bbCollisionNY( Object *o,int index );
float BBCALL bbCollisionNZ( Object *o,int index );
float BBCALL bbCollisionTime( Object *o,int index );
Object * BBCALL bbCollisionEntity( Object *o,int index );
void * BBCALL bbCollisionSurface( Object *o,int index );
int BBCALL bbCollisionTriangle( Object *o,int index );
float BBCALL BBCALL bbEntityDistance( Entity *src,Entity *dest );

////////////////////////////////////
// ENTITY TRANSFORMATION COMMANDS //
////////////////////////////////////
void BBCALL bbMoveEntity( Entity *e,float x,float y,float z );
void BBCALL bbTurnEntity( Entity *e,float p,float y,float r,int global );
void BBCALL bbTranslateEntity( Entity *e,float x,float y,float z,int global );
void BBCALL bbPositionEntity( Entity *e,float x,float y,float z,int global );
void BBCALL bbScaleEntity( Entity *e,float x,float y,float z,int global );
void BBCALL bbRotateEntity( Entity *e,float p,float y,float r,int global );
void BBCALL bbPointEntity( Entity *e,Entity *t,float roll );
void BBCALL bbAlignToVector( Entity *e,float nx,float ny,float nz,int axis,float rate );

//////////////////////////
// ENTITY MISC COMMANDS //
//////////////////////////
void BBCALL bbNameEntity( Entity *e,BBStr *t );
BBStr * BBCALL bbEntityName( Entity *e );
BBStr * BBCALL bbEntityClass( Entity *e );
void BBCALL bbClearWorld( int e,int b,int t );
int BBCALL bbActiveTextures();


#ifdef __cplusplus
}
#endif

#endif
