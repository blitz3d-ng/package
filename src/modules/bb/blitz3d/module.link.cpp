// AUTOGENERATED. DO NOT EDIT.
// RUN `bin/blitz3d config` TO UPDATE.

#include <bb/blitz/module.h>
#include <bb/blitz3d/blitz3d.h>

#ifdef WIN32
BBMODULE_LINK( blitz3d ){
	rtSym( "%ActiveTextures",bbActiveTextures );
	rtSym( "%AddAnimSeq%entity%length",bbAddAnimSeq );
	rtSym( "AddMesh%source_mesh%dest_mesh",bbAddMesh );
	rtSym( "%AddTriangle%surface%v0%v1%v2",bbAddTriangle );
	rtSym( "%AddVertex%surface#x#y#z#u=0#v=0#w=1",bbAddVertex );
	rtSym( "AlignToVector%entity#vector_x#vector_y#vector_z%axis#rate=1",bbAlignToVector );
	rtSym( "AmbientLight#red#green#blue",bbAmbientLight );
	rtSym( "%AnimLength%entity",bbAnimLength );
	rtSym( "%AnimSeq%entity",bbAnimSeq );
	rtSym( "#AnimTime%entity",bbAnimTime );
	rtSym( "Animate%entity%mode=1#speed=1%sequence=0#transition=0",bbAnimate );
	rtSym( "AnimateMD2%md2%mode=1#speed=1%first_frame=0%last_frame=9999#transition=0",bbAnimateMD2 );
	rtSym( "%Animating%entity",bbAnimating );
	rtSym( "AntiAlias%enable",bbAntiAlias );
	rtSym( "BSPAmbientLight%bsp#red#green#blue",bbBSPAmbientLight );
	rtSym( "BSPLighting%bsp%use_lightmaps",bbBSPLighting );
	rtSym( "BrushAlpha%brush#alpha",bbBrushAlpha );
	rtSym( "BrushBlend%brush%blend",bbBrushBlend );
	rtSym( "BrushColor%brush#red#green#blue",bbBrushColor );
	rtSym( "BrushFX%brush%fx",bbBrushFX );
	rtSym( "BrushShininess%brush#shininess",bbBrushShininess );
	rtSym( "BrushTexture%brush%texture%frame=0%index=0",bbBrushTexture );
	rtSym( "CameraClsColor%camera#red#green#blue",bbCameraClsColor );
	rtSym( "CameraClsMode%camera%cls_color%cls_zbuffer",bbCameraClsMode );
	rtSym( "CameraFogColor%camera#red#green#blue",bbCameraFogColor );
	rtSym( "CameraFogMode%camera%mode",bbCameraFogMode );
	rtSym( "CameraFogRange%camera#near#far",bbCameraFogRange );
	rtSym( "%CameraPick%camera#viewport_x#viewport_y",bbCameraPick );
	rtSym( "CameraProjMode%camera%mode",bbCameraProjMode );
	rtSym( "CameraProject%camera#x#y#z",bbCameraProject );
	rtSym( "CameraRange%camera#near#far",bbCameraRange );
	rtSym( "CameraViewport%camera%x%y%width%height",bbCameraViewport );
	rtSym( "CameraZoom%camera#zoom",bbCameraZoom );
	rtSym( "CaptureWorld",bbCaptureWorld );
	rtSym( "ClearCollisions",bbClearCollisions );
	rtSym( "ClearSurface%surface%clear_vertices=1%clear_triangles=1",bbClearSurface );
	rtSym( "ClearTextureFilters",bbClearTextureFilters );
	rtSym( "ClearWorld%entities=1%brushes=1%textures=1",bbClearWorld );
	rtSym( "%CollisionEntity%entity%collision_index",bbCollisionEntity );
	rtSym( "#CollisionNX%entity%collision_index",bbCollisionNX );
	rtSym( "#CollisionNY%entity%collision_index",bbCollisionNY );
	rtSym( "#CollisionNZ%entity%collision_index",bbCollisionNZ );
	rtSym( "%CollisionSurface%entity%collision_index",bbCollisionSurface );
	rtSym( "#CollisionTime%entity%collision_index",bbCollisionTime );
	rtSym( "%CollisionTriangle%entity%collision_index",bbCollisionTriangle );
	rtSym( "#CollisionX%entity%collision_index",bbCollisionX );
	rtSym( "#CollisionY%entity%collision_index",bbCollisionY );
	rtSym( "#CollisionZ%entity%collision_index",bbCollisionZ );
	rtSym( "Collisions%source_type%destination_type%method%response",bbCollisions );
	rtSym( "%CopyEntity%entity%parent=0",bbCopyEntity );
	rtSym( "%CopyMesh%mesh%parent=0",bbCopyMesh );
	rtSym( "%CountChildren%entity",bbCountChildren );
	rtSym( "%CountCollisions%entity",bbCountCollisions );
	rtSym( "%CountSurfaces%mesh",bbCountSurfaces );
	rtSym( "%CountTriangles%surface",bbCountTriangles );
	rtSym( "%CountVertices%surface",bbCountVertices );
	rtSym( "%CreateBrush#red=255#green=255#blue=255",bbCreateBrush );
	rtSym( "%CreateCamera%parent=0",bbCreateCamera );
	rtSym( "%CreateCone%segments=8%solid=1%parent=0",bbCreateCone );
	rtSym( "%CreateCube%parent=0",bbCreateCube );
	rtSym( "%CreateCylinder%segments=8%solid=1%parent=0",bbCreateCylinder );
	rtSym( "%CreateLight%type=1%parent=0",bbCreateLight );
	rtSym( "%CreateListener%parent#rolloff_factor=1#doppler_scale=1#distance_scale=1",bbCreateListener );
	rtSym( "%CreateMesh%parent=0",bbCreateMesh );
	rtSym( "%CreateMirror%parent=0",bbCreateMirror );
	rtSym( "%CreatePivot%parent=0",bbCreatePivot );
	rtSym( "%CreatePlane%segments=1%parent=0",bbCreatePlane );
	rtSym( "%CreateSphere%segments=8%parent=0",bbCreateSphere );
	rtSym( "%CreateSprite%parent=0",bbCreateSprite );
	rtSym( "%CreateSurface%mesh%brush=0",bbCreateSurface );
	rtSym( "%CreateTerrain%grid_size%parent=0",bbCreateTerrain );
	rtSym( "%CreateTexture%width%height%flags=0%frames=1",bbCreateTexture );
	rtSym( "#DeltaPitch%src_entity%dest_entity",bbDeltaPitch );
	rtSym( "#DeltaYaw%src_entity%dest_entity",bbDeltaYaw );
	rtSym( "Dither%enable",bbDither );
	rtSym( "%EmitSound%sound%entity",bbEmitSound );
	rtSym( "EntityAlpha%entity#alpha",bbEntityAlpha );
	rtSym( "EntityAutoFade%entity#near#far",bbEntityAutoFade );
	rtSym( "EntityBlend%entity%blend",bbEntityBlend );
	rtSym( "EntityBox%entity#x#y#z#width#height#depth",bbEntityBox );
	rtSym( "$EntityClass%entity",bbEntityClass );
	rtSym( "%EntityCollided%entity%type",bbEntityCollided );
	rtSym( "EntityColor%entity#red#green#blue",bbEntityColor );
	rtSym( "#EntityDistance%source_entity%destination_entity",bbEntityDistance );
	rtSym( "EntityFX%entity%fx",bbEntityFX );
	rtSym( "%EntityInView%entity%camera",bbEntityInView );
	rtSym( "$EntityName%entity",bbEntityName );
	rtSym( "EntityOrder%entity%order",bbEntityOrder );
	rtSym( "EntityParent%entity%parent%global=1",bbEntityParent );
	rtSym( "%EntityPick%entity#range",bbEntityPick );
	rtSym( "EntityPickMode%entity%pick_geometry%obscurer=1",bbEntityPickMode );
	rtSym( "#EntityPitch%entity%global=0",bbEntityPitch );
	rtSym( "EntityRadius%entity#x_radius#y_radius=0",bbEntityRadius );
	rtSym( "#EntityRoll%entity%global=0",bbEntityRoll );
	rtSym( "EntityShininess%entity#shininess",bbEntityShininess );
	rtSym( "EntityTexture%entity%texture%frame=0%index=0",bbEntityTexture );
	rtSym( "EntityType%entity%collision_type%recursive=0",bbEntityType );
	rtSym( "%EntityVisible%src_entity%dest_entity",bbEntityVisible );
	rtSym( "#EntityX%entity%global=0",bbEntityX );
	rtSym( "#EntityY%entity%global=0",bbEntityY );
	rtSym( "#EntityYaw%entity%global=0",bbEntityYaw );
	rtSym( "#EntityZ%entity%global=0",bbEntityZ );
	rtSym( "%ExtractAnimSeq%entity%first_frame%last_frame%anim_seq=0",bbExtractAnimSeq );
	rtSym( "%FindChild%entity$name",bbFindChild );
	rtSym( "%FindSurface%mesh%brush",bbFindSurface );
	rtSym( "FitMesh%mesh#x#y#z#width#height#depth%uniform=0",bbFitMesh );
	rtSym( "FlipMesh%mesh",bbFlipMesh );
	rtSym( "FreeBrush%brush",bbFreeBrush );
	rtSym( "FreeEntity%entity",bbFreeEntity );
	rtSym( "FreeTexture%texture",bbFreeTexture );
	rtSym( "#GetBrushAlpha%brush",bbGetBrushAlpha );
	rtSym( "%GetBrushBlend%brush",bbGetBrushBlend );
	rtSym( "%GetBrushBlue%brush",bbGetBrushBlue );
	rtSym( "%GetBrushFX%brush",bbGetBrushFX );
	rtSym( "%GetBrushGreen%brush",bbGetBrushGreen );
	rtSym( "%GetBrushRed%brush",bbGetBrushRed );
	rtSym( "#GetBrushShininess%brush",bbGetBrushShininess );
	rtSym( "%GetBrushTexture%brush%index=0",bbGetBrushTexture );
	rtSym( "%GetChild%entity%index",bbGetChild );
	rtSym( "%GetEntityBrush%entity",bbGetEntityBrush );
	rtSym( "%GetEntityType%entity",bbGetEntityType );
	rtSym( "#GetMatElement%entity%row%column",bbGetMatElement );
	rtSym( "%GetParent%entity",bbGetParent );
	rtSym( "%GetSurface%mesh%surface_index",bbGetSurface );
	rtSym( "%GetSurfaceBrush%surface",bbGetSurfaceBrush );
	rtSym( "%GfxDriverCaps3D",bbGfxDriverCaps3D );
	rtSym( "HWMultiTex%enable",bbHWMultiTex );
	rtSym( "%HWTexUnits",bbHWTexUnits );
	rtSym( "HandleSprite%sprite#x_handle#y_handle",bbHandleSprite );
	rtSym( "HideEntity%entity",bbHideEntity );
	rtSym( "LightColor%light#red#green#blue",bbLightColor );
	rtSym( "LightConeAngles%light#inner_angle#outer_angle",bbLightConeAngles );
	rtSym( "LightMesh%mesh#red#green#blue#range=0#x=0#y=0#z=0",bbLightMesh );
	rtSym( "LightRange%light#range",bbLightRange );
	rtSym( "%LinePick#x#y#z#dx#dy#dz#radius=0",bbLinePick );
	rtSym( "%LoadAnimMesh$file%parent=0",bbLoadAnimMesh );
	rtSym( "%LoadAnimSeq%entity$file",bbLoadAnimSeq );
	rtSym( "%LoadAnimTexture$file%flags%width%height%first%count",bbLoadAnimTexture );
	rtSym( "%LoadBSP$file#gamma_adj=0%parent=0",bbLoadBSP );
	rtSym( "%LoadBrush$file%texture_flags=1#u_scale=1#v_scale=1",bbLoadBrush );
	rtSym( "%LoadMD2$file%parent=0",bbLoadMD2 );
	rtSym( "%LoadMesh$file%parent=0",bbLoadMesh );
	rtSym( "%LoadSprite$file%texture_flags=1%parent=0",bbLoadSprite );
	rtSym( "%LoadTerrain$heightmap_file%parent=0",bbLoadTerrain );
	rtSym( "%LoadTexture$file%flags=1",bbLoadTexture );
	rtSym( "LoaderMatrix$file_ext#xx#xy#xz#yx#yy#yz#zx#zy#zz",bbLoaderMatrix );
	rtSym( "%MD2AnimLength%md2",bbMD2AnimLength );
	rtSym( "#MD2AnimTime%md2",bbMD2AnimTime );
	rtSym( "%MD2Animating%md2",bbMD2Animating );
	rtSym( "MeshCullBox%mesh#x#y#z#width#height#depth",bbMeshCullBox );
	rtSym( "#MeshDepth%mesh",bbMeshDepth );
	rtSym( "#MeshHeight%mesh",bbMeshHeight );
	rtSym( "#MeshWidth%mesh",bbMeshWidth );
	rtSym( "%MeshesIntersect%mesh_a%mesh_b",bbMeshesIntersect );
	rtSym( "ModifyTerrain%terrain%terrain_x%terrain_z#height%realtime=0",bbModifyTerrain );
	rtSym( "MoveEntity%entity#x#y#z",bbMoveEntity );
	rtSym( "NameEntity%entity$name",bbNameEntity );
	rtSym( "PaintEntity%entity%brush",bbPaintEntity );
	rtSym( "PaintMesh%mesh%brush",bbPaintMesh );
	rtSym( "PaintSurface%surface%brush",bbPaintSurface );
	rtSym( "%PickedEntity",bbPickedEntity );
	rtSym( "#PickedNX",bbPickedNX );
	rtSym( "#PickedNY",bbPickedNY );
	rtSym( "#PickedNZ",bbPickedNZ );
	rtSym( "%PickedSurface",bbPickedSurface );
	rtSym( "#PickedTime",bbPickedTime );
	rtSym( "%PickedTriangle",bbPickedTriangle );
	rtSym( "#PickedX",bbPickedX );
	rtSym( "#PickedY",bbPickedY );
	rtSym( "#PickedZ",bbPickedZ );
	rtSym( "PointEntity%entity%target#roll=0",bbPointEntity );
	rtSym( "PositionEntity%entity#x#y#z%global=0",bbPositionEntity );
	rtSym( "PositionMesh%mesh#x#y#z",bbPositionMesh );
	rtSym( "PositionTexture%texture#u_offset#v_offset",bbPositionTexture );
	rtSym( "#ProjectedX",bbProjectedX );
	rtSym( "#ProjectedY",bbProjectedY );
	rtSym( "#ProjectedZ",bbProjectedZ );
	rtSym( "RenderWorld#tween=1",bbRenderWorld );
	rtSym( "ResetEntity%entity",bbResetEntity );
	rtSym( "RotateEntity%entity#pitch#yaw#roll%global=0",bbRotateEntity );
	rtSym( "RotateMesh%mesh#pitch#yaw#roll",bbRotateMesh );
	rtSym( "RotateSprite%sprite#angle",bbRotateSprite );
	rtSym( "RotateTexture%texture#angle",bbRotateTexture );
	rtSym( "ScaleEntity%entity#x_scale#y_scale#z_scale%global=0",bbScaleEntity );
	rtSym( "ScaleMesh%mesh#x_scale#y_scale#z_scale",bbScaleMesh );
	rtSym( "ScaleSprite%sprite#x_scale#y_scale",bbScaleSprite );
	rtSym( "ScaleTexture%texture#u_scale#v_scale",bbScaleTexture );
	rtSym( "SetAnimKey%entity%frame%pos_key=1%rot_key=1%scale_key=1",bbSetAnimKey );
	rtSym( "SetAnimTime%entity#time%anim_seq=0",bbSetAnimTime );
	rtSym( "SetCubeFace%texture%face",bbSetCubeFace );
	rtSym( "SetCubeMode%texture%mode",bbSetCubeMode );
	rtSym( "ShowEntity%entity",bbShowEntity );
	rtSym( "SpriteViewMode%sprite%view_mode",bbSpriteViewMode );
	rtSym( "#Stats3D%type",bbStats3D );
	rtSym( "TFormNormal#x#y#z%source_entity%dest_entity",bbTFormNormal );
	rtSym( "TFormPoint#x#y#z%source_entity%dest_entity",bbTFormPoint );
	rtSym( "TFormVector#x#y#z%source_entity%dest_entity",bbTFormVector );
	rtSym( "#TFormedX",bbTFormedX );
	rtSym( "#TFormedY",bbTFormedY );
	rtSym( "#TFormedZ",bbTFormedZ );
	rtSym( "TerrainDetail%terrain%detail_level%morph=0",bbTerrainDetail );
	rtSym( "#TerrainHeight%terrain%terrain_x%terrain_z",bbTerrainHeight );
	rtSym( "TerrainShading%terrain%enable",bbTerrainShading );
	rtSym( "%TerrainSize%terrain",bbTerrainSize );
	rtSym( "#TerrainX%terrain#world_x#world_y#world_z",bbTerrainX );
	rtSym( "#TerrainY%terrain#world_x#world_y#world_z",bbTerrainY );
	rtSym( "#TerrainZ%terrain#world_x#world_y#world_z",bbTerrainZ );
	rtSym( "TextureBlend%texture%blend",bbTextureBlend );
	rtSym( "%TextureBuffer%texture%frame=0",bbTextureBuffer );
	rtSym( "TextureCoords%texture%coords",bbTextureCoords );
	rtSym( "TextureFilter$match_text%texture_flags=0",bbTextureFilter );
	rtSym( "%TextureHeight%texture",bbTextureHeight );
	rtSym( "$TextureName%texture",bbTextureName );
	rtSym( "%TextureWidth%texture",bbTextureWidth );
	rtSym( "TranslateEntity%entity#x#y#z%global=0",bbTranslateEntity );
	rtSym( "%TriangleVertex%surface%index%vertex",bbTriangleVertex );
	rtSym( "%TrisRendered",bbTrisRendered );
	rtSym( "TurnEntity%entity#pitch#yaw#roll%global=0",bbTurnEntity );
	rtSym( "UpdateNormals%mesh",bbUpdateNormals );
	rtSym( "UpdateWorld#elapsed_time=1",bbUpdateWorld );
	rtSym( "#VectorPitch#x#y#z",bbVectorPitch );
	rtSym( "#VectorYaw#x#y#z",bbVectorYaw );
	rtSym( "#VertexAlpha%surface%index",bbVertexAlpha );
	rtSym( "#VertexBlue%surface%index",bbVertexBlue );
	rtSym( "VertexColor%surface%index#red#green#blue#alpha=1",bbVertexColor );
	rtSym( "VertexCoords%surface%index#x#y#z",bbVertexCoords );
	rtSym( "#VertexGreen%surface%index",bbVertexGreen );
	rtSym( "#VertexNX%surface%index",bbVertexNX );
	rtSym( "#VertexNY%surface%index",bbVertexNY );
	rtSym( "#VertexNZ%surface%index",bbVertexNZ );
	rtSym( "VertexNormal%surface%index#nx#ny#nz",bbVertexNormal );
	rtSym( "#VertexRed%surface%index",bbVertexRed );
	rtSym( "VertexTexCoords%surface%index#u#v#w=1%coord_set=0",bbVertexTexCoords );
	rtSym( "#VertexU%surface%index%coord_set=0",bbVertexU );
	rtSym( "#VertexV%surface%index%coord_set=0",bbVertexV );
	rtSym( "#VertexW%surface%index%coord_set=0",bbVertexW );
	rtSym( "#VertexX%surface%index",bbVertexX );
	rtSym( "#VertexY%surface%index",bbVertexY );
	rtSym( "#VertexZ%surface%index",bbVertexZ );
	rtSym( "WBuffer%enable",bbWBuffer );
	rtSym( "WireFrame%enable",bbWireFrame );
}
#endif
