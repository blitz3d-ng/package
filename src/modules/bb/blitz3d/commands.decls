LoaderMatrix( file_ext$,xx#,xy#,xz#,yx#,yy#,yz#,zx#,zy#,zz# ):"bbLoaderMatrix"
HWMultiTex( enable% ):"bbHWMultiTex"
HWTexUnits%():"bbHWTexUnits"
GfxDriverCaps3D%():"bbGfxDriverCaps3D"
WBuffer( enable% ):"bbWBuffer"
Dither( enable% ):"bbDither"
AntiAlias( enable% ):"bbAntiAlias"
WireFrame( enable% ):"bbWireFrame"
AmbientLight( red#,green#,blue# ):"bbAmbientLight"
ClearCollisions():"bbClearCollisions"
Collisions( source_type%,destination_type%,method%,response% ):"bbCollisions"
UpdateWorld( elapsed_time#=1 ):"bbUpdateWorld"
CaptureWorld():"bbCaptureWorld"
RenderWorld( tween#=1 ):"bbRenderWorld"
ClearWorld( entities%=1,brushes%=1,textures%=1 ):"bbClearWorld"
ActiveTextures%():"bbActiveTextures"
TrisRendered%():"bbTrisRendered"
Stats3D#( type% ):"bbStats3D"

CreateTexture.Texture( width%,height%,flags%=0,frames%=1 ):"bbCreateTexture"
LoadTexture.Texture( file$,flags%=1 ):"bbLoadTexture"
LoadAnimTexture.Texture( file$,flags%,width%,height%,first%,count% ):"bbLoadAnimTexture"
FreeTexture( texture.Texture ):"bbFreeTexture"
TextureBlend( texture.Texture,blend% ):"bbTextureBlend"
TextureCoords( texture.Texture,coords% ):"bbTextureCoords"
ScaleTexture( texture.Texture,u_scale#,v_scale# ):"bbScaleTexture"
RotateTexture( texture.Texture,angle# ):"bbRotateTexture"
PositionTexture( texture.Texture,u_offset#,v_offset# ):"bbPositionTexture"
TextureWidth%( texture.Texture ):"bbTextureWidth"
TextureHeight%( texture.Texture ):"bbTextureHeight"
TextureName$( texture.Texture ):"bbTextureName"
SetCubeFace( texture.Texture,face% ):"bbSetCubeFace"
SetCubeMode( texture.Texture,mode% ):"bbSetCubeMode"
TextureBuffer.BBCanvas( texture.Texture,frame%=0 ):"bbTextureBuffer"
ClearTextureFilters():"bbClearTextureFilters"
TextureFilter( match_text$,texture_flags%=0 ):"bbTextureFilter"

CreateBrush.Brush( red#=255,green#=255,blue#=255 ):"bbCreateBrush"
LoadBrush.Brush( file$,texture_flags%=1,u_scale#=1,v_scale#=1 ):"bbLoadBrush"
FreeBrush( brush.Brush ):"bbFreeBrush"
BrushColor( brush.Brush,red#,green#,blue# ):"bbBrushColor"
GetBrushRed%( brush.Brush ):"bbGetBrushRed"
GetBrushGreen%( brush.Brush ):"bbGetBrushGreen"
GetBrushBlue%( brush.Brush ):"bbGetBrushBlue"
BrushAlpha( brush.Brush,alpha# ):"bbBrushAlpha"
GetBrushAlpha#( brush.Brush ):"bbGetBrushAlpha"
BrushShininess( brush.Brush,shininess# ):"bbBrushShininess"
GetBrushShininess#( brush.Brush ):"bbGetBrushShininess"
BrushTexture( brush.Brush,texture.Texture,frame%=0,index%=0 ):"bbBrushTexture"
GetBrushTexture.Texture( brush.Brush,index%=0 ):"bbGetBrushTexture"
BrushBlend( brush.Brush,blend% ):"bbBrushBlend"
GetBrushBlend%( brush.Brush ):"bbGetBrushBlend"
BrushFX( brush.Brush,fx% ):"bbBrushFX"
GetBrushFX%( brush.Brush ):"bbGetBrushFX"

LoadMesh.Entity( file$,parent.Entity=0 ):"bbLoadMesh"
LoadAnimMesh.Entity( file$,parent.Entity=0 ):"bbLoadAnimMesh"
LoadAnimSeq%( entity.Object,file$ ):"bbLoadAnimSeq"

CreateMesh.Entity( parent.Entity=0 ):"bbCreateMesh"
CreateCube.Entity( parent.Entity=0 ):"bbCreateCube"
CreateSphere.Entity( segments%=8,parent.Entity=0 ):"bbCreateSphere"
CreateCylinder.Entity( segments%=8,solid%=1,parent.Entity=0 ):"bbCreateCylinder"
CreateCone.Entity( segments%=8,solid%=1,parent.Entity=0 ):"bbCreateCone"
CopyMesh.Entity( mesh.MeshModel,parent.Entity=0 ):"bbCopyMesh"
ScaleMesh( mesh.MeshModel,x_scale#,y_scale#,z_scale# ):"bbScaleMesh"
RotateMesh( mesh.MeshModel,pitch#,yaw#,roll# ):"bbRotateMesh"
PositionMesh( mesh.MeshModel,x#,y#,z# ):"bbPositionMesh"
FitMesh( mesh.MeshModel,x#,y#,z#,width#,height#,depth#,uniform%=0 ):"bbFitMesh"
FlipMesh( mesh.MeshModel ):"bbFlipMesh"
PaintMesh( mesh.MeshModel,brush.Brush ):"bbPaintMesh"
AddMesh( source_mesh.MeshModel,dest_mesh.MeshModel ):"bbAddMesh"
UpdateNormals( mesh.MeshModel ):"bbUpdateNormals"
LightMesh( mesh.MeshModel,red#,green#,blue#,range#=0,x#=0,y#=0,z#=0 ):"bbLightMesh"
MeshWidth#( mesh.MeshModel ):"bbMeshWidth"
MeshHeight#( mesh.MeshModel ):"bbMeshHeight"
MeshDepth#( mesh.MeshModel ):"bbMeshDepth"
MeshesIntersect%( mesh_a.MeshModel,mesh_b.MeshModel ):"bbMeshesIntersect"
CountSurfaces%( mesh.MeshModel ):"bbCountSurfaces"
GetSurface.Surface( mesh.MeshModel,surface_index% ):"bbGetSurface"
MeshCullBox( mesh.MeshModel,x#,y#,z#,width#,height#,depth# ):"bbMeshCullBox"

CreateSurface.Surface( mesh.MeshModel,brush.Brush=0 ):"bbCreateSurface"
GetSurfaceBrush.Brush( surface.Surface ):"bbGetSurfaceBrush"
GetEntityBrush.Brush( entity.Model ):"bbGetEntityBrush"
FindSurface.Surface( mesh.MeshModel,brush.Brush ):"bbFindSurface"
ClearSurface( surface.Surface,clear_vertices%=1,clear_triangles%=1 ):"bbClearSurface"
PaintSurface( surface.Surface,brush.Brush ):"bbPaintSurface"
AddVertex%( surface.Surface,x#,y#,z#,u#=0,v#=0,w#=1 ):"bbAddVertex"
AddTriangle%( surface.Surface,v0%,v1%,v2% ):"bbAddTriangle"
VertexCoords( surface.Surface,index%,x#,y#,z# ):"bbVertexCoords"
VertexNormal( surface.Surface,index%,nx#,ny#,nz# ):"bbVertexNormal"
VertexColor( surface.Surface,index%,red#,green#,blue#,alpha#=1 ):"bbVertexColor"
VertexTexCoords( surface.Surface,index%,u#,v#,w#=1,coord_set%=0 ):"bbVertexTexCoords"
CountVertices%( surface.Surface ):"bbCountVertices"
CountTriangles%( surface.Surface ):"bbCountTriangles"
VertexX#( surface.Surface,index% ):"bbVertexX"
VertexY#( surface.Surface,index% ):"bbVertexY"
VertexZ#( surface.Surface,index% ):"bbVertexZ"
VertexNX#( surface.Surface,index% ):"bbVertexNX"
VertexNY#( surface.Surface,index% ):"bbVertexNY"
VertexNZ#( surface.Surface,index% ):"bbVertexNZ"
VertexRed#( surface.Surface,index% ):"bbVertexRed"
VertexGreen#( surface.Surface,index% ):"bbVertexGreen"
VertexBlue#( surface.Surface,index% ):"bbVertexBlue"
VertexAlpha#( surface.Surface,index% ):"bbVertexAlpha"
VertexU#( surface.Surface,index%,coord_set%=0 ):"bbVertexU"
VertexV#( surface.Surface,index%,coord_set%=0 ):"bbVertexV"
VertexW#( surface.Surface,index%,coord_set%=0 ):"bbVertexW"
TriangleVertex%( surface.Surface,index%,vertex% ):"bbTriangleVertex"

CreateCamera.Entity( parent.Entity=0 ):"bbCreateCamera"
CameraZoom( camera.Camera,zoom# ):"bbCameraZoom"
CameraRange( camera.Camera,near#,far# ):"bbCameraRange"
CameraClsColor( camera.Camera,red#,green#,blue# ):"bbCameraClsColor"
CameraClsMode( camera.Camera,cls_color%,cls_zbuffer% ):"bbCameraClsMode"
CameraProjMode( camera.Camera,mode% ):"bbCameraProjMode"
CameraViewport( camera.Camera,x%,y%,width%,height% ):"bbCameraViewport"
CameraFogColor( camera.Camera,red#,green#,blue# ):"bbCameraFogColor"
CameraFogRange( camera.Camera,near#,far# ):"bbCameraFogRange"
CameraFogMode( camera.Camera,mode% ):"bbCameraFogMode"
CameraProject%( camera.Camera,x#,y#,z# ):"bbCameraProject"
ProjectedX#():"bbProjectedX"
ProjectedY#():"bbProjectedY"
ProjectedZ#():"bbProjectedZ"

EntityInView%( entity.Entity,camera.Camera ):"bbEntityInView"
EntityVisible%( src_entity.Object,dest_entity.Object ):"bbEntityVisible"

EntityPick.Entity( entity.Object,range# ):"bbEntityPick"
LinePick.Entity( x#,y#,z#,dx#,dy#,dz#,radius#=0 ):"bbLinePick"
CameraPick.Entity( camera.Camera,viewport_x#,viewport_y# ):"bbCameraPick"

PickedX#():"bbPickedX"
PickedY#():"bbPickedY"
PickedZ#():"bbPickedZ"
PickedNX#():"bbPickedNX"
PickedNY#():"bbPickedNY"
PickedNZ#():"bbPickedNZ"
PickedTime#():"bbPickedTime"
PickedEntity.Object():"bbPickedEntity"
PickedSurface.Surface():"bbPickedSurface"
PickedTriangle%():"bbPickedTriangle"

CreateLight.Entity( type%=1,parent.Entity=0 ):"bbCreateLight"
LightColor( light.Light,red#,green#,blue# ):"bbLightColor"
LightRange( light.Light,range# ):"bbLightRange"
LightConeAngles( light.Light,inner_angle#,outer_angle# ):"bbLightConeAngles"

CreatePivot.Entity( parent.Entity=0 ):"bbCreatePivot"

CreateSprite.Entity( parent.Entity=0 ):"bbCreateSprite"
LoadSprite.Entity( file$,texture_flags%=1,parent.Entity=0 ):"bbLoadSprite"
RotateSprite( sprite.Sprite,angle# ):"bbRotateSprite"
ScaleSprite( sprite.Sprite,x_scale#,y_scale# ):"bbScaleSprite"
HandleSprite( sprite.Sprite,x_handle#,y_handle# ):"bbHandleSprite"
SpriteViewMode( sprite.Sprite,view_mode% ):"bbSpriteViewMode"

LoadMD2.Entity( file$,parent.Entity=0 ):"bbLoadMD2"
AnimateMD2( md2.MD2Model,mode%=1,speed#=1,first_frame%=0,last_frame%=9999,transition#=0 ):"bbAnimateMD2"
MD2AnimTime#( md2.MD2Model ):"bbMD2AnimTime"
MD2AnimLength%( md2.MD2Model ):"bbMD2AnimLength"
MD2Animating%( md2.MD2Model ):"bbMD2Animating"

LoadBSP.Entity( file$,gamma_adj#=0,parent.Entity=0 ):"bbLoadBSP"
BSPLighting( bsp.Q3BSPModel,use_lightmaps% ):"bbBSPLighting"
BSPAmbientLight( bsp.Q3BSPModel,red#,green#,blue# ):"bbBSPAmbientLight"

CreateMirror.Entity( parent.Entity=0 ):"bbCreateMirror"

CreatePlane.Entity( segments%=1,parent.Entity=0 ):"bbCreatePlane"

CreateTerrain.Entity( grid_size%,parent.Entity=0 ):"bbCreateTerrain"
LoadTerrain.Entity( heightmap_file$,parent.Entity=0 ):"bbLoadTerrain"
TerrainDetail( terrain.Terrain,detail_level%,morph%=0 ):"bbTerrainDetail"
TerrainShading( terrain.Terrain,enable% ):"bbTerrainShading"
TerrainX#( terrain.Terrain,world_x#,world_y#,world_z# ):"bbTerrainX"
TerrainY#( terrain.Terrain,world_x#,world_y#,world_z# ):"bbTerrainY"
TerrainZ#( terrain.Terrain,world_x#,world_y#,world_z# ):"bbTerrainZ"
TerrainSize%( terrain.Terrain ):"bbTerrainSize"
TerrainHeight#( terrain.Terrain,terrain_x%,terrain_z% ):"bbTerrainHeight"
ModifyTerrain( terrain.Terrain,terrain_x%,terrain_z%,height#,realtime%=0 ):"bbModifyTerrain"

CreateListener.Entity( parent.Entity,rolloff_factor#=1,doppler_scale#=1,distance_scale#=1 ):"bbCreateListener"
EmitSound.BBChannel( sound.BBSound,entity.Object ):"bbEmitSound"

CopyEntity.Entity( entity.Entity,parent.Entity=0 ):"bbCopyEntity"

EntityX#( entity.Entity,global%=0 ):"bbEntityX"
EntityY#( entity.Entity,global%=0 ):"bbEntityY"
EntityZ#( entity.Entity,global%=0 ):"bbEntityZ"
EntityPitch#( entity.Entity,global%=0 ):"bbEntityPitch"
EntityYaw#( entity.Entity,global%=0 ):"bbEntityYaw"
EntityRoll#( entity.Entity,global%=0 ):"bbEntityRoll"
GetMatElement#( entity.Entity,row%,column% ):"bbGetMatElement"
TFormPoint( x#,y#,z#,source_entity.Entity,dest_entity.Entity ):"bbTFormPoint"
TFormVector( x#,y#,z#,source_entity.Entity,dest_entity.Entity ):"bbTFormVector"
TFormNormal( x#,y#,z#,source_entity.Entity,dest_entity.Entity ):"bbTFormNormal"
TFormedX#():"bbTFormedX"
TFormedY#():"bbTFormedY"
TFormedZ#():"bbTFormedZ"
VectorYaw#( x#,y#,z# ):"bbVectorYaw"
VectorPitch#( x#,y#,z# ):"bbVectorPitch"
DeltaPitch#( src_entity.Entity,dest_entity.Entity ):"bbDeltaPitch"
DeltaYaw#( src_entity.Entity,dest_entity.Entity ):"bbDeltaYaw"

ResetEntity( entity.Object ):"bbResetEntity"
EntityType( entity.Object,collision_type%,recursive%=0 ):"bbEntityType"
EntityPickMode( entity.Object,pick_geometry%,obscurer%=1 ):"bbEntityPickMode"
GetParent.Entity( entity.Entity ):"bbGetParent"
GetEntityType%( entity.Object ):"bbGetEntityType"
EntityRadius( entity.Object,x_radius#,y_radius#=0 ):"bbEntityRadius"
EntityBox( entity.Object,x#,y#,z#,width#,height#,depth# ):"bbEntityBox"
EntityDistance#( source_entity.Entity,destination_entity.Entity ):"bbEntityDistance"
EntityCollided.Object( entity.Object,type% ):"bbEntityCollided"

CountCollisions%( entity.Object ):"bbCountCollisions"
CollisionX#( entity.Object,collision_index% ):"bbCollisionX"
CollisionY#( entity.Object,collision_index% ):"bbCollisionY"
CollisionZ#( entity.Object,collision_index% ):"bbCollisionZ"
CollisionNX#( entity.Object,collision_index% ):"bbCollisionNX"
CollisionNY#( entity.Object,collision_index% ):"bbCollisionNY"
CollisionNZ#( entity.Object,collision_index% ):"bbCollisionNZ"
CollisionTime#( entity.Object,collision_index% ):"bbCollisionTime"
CollisionEntity.Object( entity.Object,collision_index% ):"bbCollisionEntity"
CollisionSurface.Surface( entity.Object,collision_index% ):"bbCollisionSurface"
CollisionTriangle%( entity.Object,collision_index% ):"bbCollisionTriangle"

MoveEntity( entity.Entity,x#,y#,z# ):"bbMoveEntity"
TurnEntity( entity.Entity,pitch#,yaw#,roll#,global%=0 ):"bbTurnEntity"
TranslateEntity( entity.Entity,x#,y#,z#,global%=0 ):"bbTranslateEntity"
PositionEntity( entity.Entity,x#,y#,z#,global%=0 ):"bbPositionEntity"
ScaleEntity( entity.Entity,x_scale#,y_scale#,z_scale#,global%=0 ):"bbScaleEntity"
RotateEntity( entity.Entity,pitch#,yaw#,roll#,global%=0 ):"bbRotateEntity"
PointEntity( entity.Entity,target.Entity,roll#=0 ):"bbPointEntity"
AlignToVector( entity.Entity,vector_x#,vector_y#,vector_z#,axis%,rate#=1 ):"bbAlignToVector"
SetAnimTime( entity.Object,time#,anim_seq%=0 ):"bbSetAnimTime"
Animate( entity.Object,mode%=1,speed#=1,sequence%=0,transition#=0 ):"bbAnimate"
SetAnimKey( entity.Object,frame%,pos_key%=1,rot_key%=1,scale_key%=1 ):"bbSetAnimKey"
AddAnimSeq%( entity.Object,length% ):"bbAddAnimSeq"
ExtractAnimSeq%( entity.Object,first_frame%,last_frame%,anim_seq%=0 ):"bbExtractAnimSeq"
AnimSeq%( entity.Object ):"bbAnimSeq"
AnimTime#( entity.Object ):"bbAnimTime"
AnimLength%( entity.Object ):"bbAnimLength"
Animating%( entity.Object ):"bbAnimating"

EntityParent( entity.Entity,parent.Entity,global%=1 ):"bbEntityParent"
CountChildren%( entity.Entity ):"bbCountChildren"
GetChild.Entity( entity.Entity,index% ):"bbGetChild"
FindChild.Entity( entity.Entity,name$ ):"bbFindChild"

PaintEntity( entity.Model,brush.Brush ):"bbPaintEntity"
EntityColor( entity.Model,red#,green#,blue# ):"bbEntityColor"
EntityAlpha( entity.Model,alpha# ):"bbEntityAlpha"
EntityShininess( entity.Model,shininess# ):"bbEntityShininess"
EntityTexture( entity.Model,texture.Texture,frame%=0,index%=0 ):"bbEntityTexture"
EntityBlend( entity.Model,blend% ):"bbEntityBlend"
EntityFX( entity.Model,fx% ):"bbEntityFX"
EntityAutoFade( entity.Model,near#,far# ):"bbEntityAutoFade"
EntityOrder( entity.Object,order% ):"bbEntityOrder"
HideEntity( entity.Entity ):"bbHideEntity"
ShowEntity( entity.Entity ):"bbShowEntity"
FreeEntity( entity.Entity ):"bbFreeEntity"

NameEntity( entity.Entity,name$ ):"bbNameEntity"
EntityName$( entity.Entity ):"bbEntityName"
EntityClass$( entity.Entity ):"bbEntityClass"
