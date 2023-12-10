
Context "Blitz3D"

SetRenderer "OpenGL"
Graphics3D 800,600,32,2

ExpectInt GfxDriverCaps3D(),110

brush = CreateBrush()
Expect brush <> 0, "Can create a brush"
BrushFX brush,1
Expect GetBrushFX(brush) = 1, ""
FreeBrush brush

light = CreateLight()
Expect light <> 0, "Can create a light"

point_light = CreateLight(2)
Expect point_light <> 0, "Can create a light"

pivot = CreatePivot()
Expect pivot <> 0, "Can create a pivot"

pivot2 = CreatePivot()
Expect CopyEntity( pivot2 )<>0, "Can copy a pivot"

camera = CreateCamera()
Expect camera <> 0, "Can create a camera"
PositionEntity camera,0,5,-100
CameraRange camera,0.1,1000
CameraClsColor camera,255,0,0
CameraProjMode camera,2
CameraViewport camera,1,1,20,20

cube = CreateCube()
Expect cube <> 0, "Can create a cube"
Expect MeshWidth(cube) = 2, "Cube has a width of 2"
Expect MeshHeight(cube) = 2, "Cube has a height of 2"
Expect MeshDepth(cube) = 2, "Cube has a depth of 2"
Expect CountSurfaces(cube) = 1, "Cube has one surface"
TurnEntity cube,45,45,0

PositionEntity cube,0,0,5
Expect EntityX(cube)=0, "x=0"
Expect EntityY(cube)=0, "y=0"
Expect EntityZ(cube)=5, "z=5"

plane=CreatePlane()
Expect plane<>0, "can create a plane"

mirror=CreateMirror()
Expect mirror<>0, "can create a mirror"

mirror_copy=CopyEntity(mirror)

Function TotalVerts( m )
  v=0
  For i=1 To CountSurfaces( m )
    v=v+CountVertices(GetSurface(m,i))
  Next
  Return v
End Function

Function TotalTris( m )
  v=0
  For i=1 To CountSurfaces( m )
    v=v+CountTriangles(GetSurface(m,i))
  Next
  Return v
End Function

dwarf_b3d=LoadAnimMesh( "media/dwarf2.b3d" )
Expect dwarf_b3d<>0, "can load animated b3d"
Animate dwarf_b3d,2,0.2

level_b3d=LoadMesh( "../_release/samples/mak/cubewater/level/test.b3d" )
Expect level_b3d<>0, "can load b3d"
ExpectInt TotalVerts(level_b3d),5251
ExpectInt TotalTris(level_b3d),4383

bot_3ds=LoadAnimMesh( "../_release/samples/mak/anim/makbot/mak_robotic.3DS" )
Expect bot_3ds<>0, "can load 3ds"
LoadAnimSeq bot_3ds,"../_release/samples/mak/anim/makbot/mak_running.3DS"

bot_x=LoadAnimMesh( "../_release/samples/mak/anim/makbot/mak_robotic.x")
Expect bot_x<>0, "can load x"
LoadAnimSeq bot_x,"../_release/samples/mak/anim/makbot/mak_running.x"

dragon=LoadMD2( "../_release/samples/mak/dragon/model/dragon.md2" );
Expect dragon<>0, "can load md2"
AnimateMD2 dragon,1,.05,0,40

bot_x_flat=LoadMesh( "../_release/samples/mak/anim/makbot/mak_robotic.x")
ExpectInt TotalVerts(bot_x_flat),1883
ExpectInt TotalTris(bot_x_flat),1554

ufo=LoadMesh( "../_release/Games/wing_ring/media/blue_ufo.X" );
Expect ufo<>0, "can load X"

Function CanLoad( file$,surfs,verts,tris )
  mesh=LoadMesh( file )
  Expect mesh<>0,"Can load "+file
  If mesh
    ExpectInt CountSurfaces( mesh ),surfs,file+" has "+surfs+" surfaces"
    ExpectInt TotalVerts( mesh ),verts,file+" has "+verts+" vertices"
    ExpectInt TotalTris( mesh ),tris,file+" has "+tris+" triangles"
  End If
End Function

CanLoad( "../_release/samples/birdie/dolphin/dolphin.x",1,285,564 )
CanLoad( "../_release/samples/birdie/dolphin/seafloor.x",1,1490,2830 )
CanLoad( "../_release/samples/birdie/dolphin/ddown.x",1,285,564 )
CanLoad( "../_release/samples/birdie/dolphin/dup.x",1,285,564 )
CanLoad( "../_release/samples/birdie/LodMesh/dolphin.x",1,285,564 )
CanLoad( "../_release/samples/birdie/Jet Tails/jet.x",3,142,199 )
CanLoad( "../_release/samples/RobHutchinson/WingPilot/objects/eyepiece.x",2,245,163 )
CanLoad( "../_release/samples/RobHutchinson/WingPilot/objects/mainring.x",1,64,64 )
CanLoad( "../_release/samples/RobHutchinson/WingPilot/objects/maincent.x",1,18,16 )
CanLoad( "../_release/samples/RobHutchinson/WingPilot/Craft1.x",4,375,328 )
CanLoad( "../_release/samples/RobHutchinson/ModelChildren/Models/facia.x",2,82,28 )
CanLoad( "../_release/samples/RobHutchinson/CraftFlare/Craft1.x",4,375,328 )
CanLoad( "../_release/samples/RobHutchinson/SkyPlateau/Models/ground.x",4,777,605 )
CanLoad( "../_release/samples/RobHutchinson/SkyPlateau/Models/icon.x",1,323,276 )
CanLoad( "../_release/samples/RobHutchinson/SkyPlateau/Models/totem.x",1,290,184 )
CanLoad( "../_release/samples/halo/Lightmap/level.x",2,996,356 )
CanLoad( "../_release/samples/halo/Lightmap/lightmap.x",1,996,356 )
CanLoad( "../_release/samples/zenith/scare/models/pistol/pistol.x",1,200,287 )
CanLoad( "../_release/samples/zenith/scare/models/level/level.x",5,759,548 )
CanLoad( "../_release/samples/zenith/scare/models/level/crate.x",2,48,24 )
CanLoad( "../_release/samples/mak/anim/makbot/mak_robotic.x",3,1883,1554 )
CanLoad( "../_release/samples/mak/anim/makbot/mak_running.x",3,1883,1554 )
CanLoad( "../_release/samples/mak/driver/car.x",5,1051,1850 )
CanLoad( "../_release/samples/mak/xfighter/biplane.x",4,1282,596 )
CanLoad( "../_release/samples/mak/castle/markio/mariorun.x",6,1052,1118 )
CanLoad( "../_release/samples/mak/castle/castle/CASTLE1.X",4,3295,1506 )
CanLoad( "../_release/samples/mak/teapot/teapot.x",1,1178,2256 )
CanLoad( "../_release/samples/si/fps/ladders.X",2,10,6 )
CanLoad( "../_release/samples/si/fps/interior.X",5,3756,1878 )
CanLoad( "../_release/samples/AGore/BirdDemo/Canyon.x",1,12090,4030 )
CanLoad( "../_release/samples/AGore/HeadDemo/Eye.x",1,162,320 )
CanLoad( "../_release/samples/AGore/HeadDemo/Head.x",1,1343,2418 )
CanLoad( "../_release/samples/Hi-Toro/Death Island/msh/747.X",6,4380,5706 )
CanLoad( "../_release/samples/Hi-Toro/Shooter/Shooter/msh/ship.x",4,254,228 )
CanLoad( "../_release/Games/wing_ring/media/pink_ufo.x",4,215,414 )
CanLoad( "../_release/Games/wing_ring/media/orange_ufo.x",4,215,414 )
CanLoad( "../_release/Games/wing_ring/media/green_ufo.x",4,215,414 )
CanLoad( "../_release/Games/wing_ring/media/yellow_ufo.X",4,215,414 )
CanLoad( "../_release/Games/wing_ring/media/mother.x",5,215,374 )
CanLoad( "../_release/Games/wing_ring/media/sphinx.x",1,276,548 )
CanLoad( "../_release/Games/wing_ring/media/stat_lib.x",2,277,538 )
CanLoad( "../_release/Games/wing_ring/media/camel.x",1,275,546 )
CanLoad( "../_release/Games/wing_ring/media/blue_ufo.X",4,215,414 )
CanLoad( "../_release/Games/wing_ring/media/plane.x",4,142,199 )
CanLoad( "../_release/help/commands/3d_examples/media/CACTUS2.X",1,36,68 )
CanLoad( "../_release/help/commands/3d_examples/media/teapot.x",1,1178,2256 )
CanLoad( "../_release/help/commands/3d_examples/media/green_ufo.x",4,215,414 )
CanLoad( "../_release/help/commands/3d_examples/media/makbot/mak_robotic.x",3,1883,1554 )
CanLoad( "../_release/help/commands/3d_examples/media/makbot/mak_running.x",3,1883,1554 )
CanLoad( "../_release/help/commands/3d_examples/media/camel.x",1,275,546 )
CanLoad( "../_release/Media/geometricks_models/supplies/supplies.x",2,24,12 )

terr=LoadTerrain( "../_release/samples/mak/castle/environ/terrain-1.jpg" )
Expect terr<>0, "can load terrain"

sprite=LoadSprite( "../_release/samples/mak/castle/sprites/Bigspark.BMP" )
Expect sprite<>0, "can load sprite"

listener=CreateListener( camera )
Expect listener<>0,"can create listener"
boom = Load3DSound( "../_release/samples/mak/castle/sounds/boom.wav" )
Expect boom<>0,"can load 3d sound"

channel=EmitSound(boom, cube)
Expect channel<>0, "can emit boom from cube"

; trigger some collision code
Collisions 1,2,2,3
EntityType camera,1
EntityType cube,2

UpdateWorld

; persp
CameraProjMode camera,1
RenderWorld

; ortho
CameraProjMode camera,2
RenderWorld

MoveEntity cube,0,-2,0
UpdateWorld

TrisRendered()

FreeEntity camera
FreeEntity plane
FreeEntity mirror
FreeEntity light
FreeEntity cube
