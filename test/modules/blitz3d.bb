
Context "Blitz3D"

SetRenderer "OpenGL"
Graphics3D 800,600,32,2

brush = CreateBrush()
Expect brush <> 0, "Can create a brush"
BrushFX brush,1
Expect GetBrushFX(brush) = 1, ""
FreeBrush brush

light = CreateLight()
Expect light <> 0, "Can create a light"

pivot = CreatePivot()
Expect pivot <> 0, "Can create a pivot"

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

dwarf_b3d=LoadAnimMesh( "media/dwarf2.b3d" )
Expect dwarf_b3d<>0, "can load animated b3d"
Animate dwarf_b3d,2,0.2

level_b3d=LoadMesh( "../_release/samples/mak/cubewater/level/test.b3d" )
Expect level_b3d<>0, "can load b3d"

bot_3ds=LoadAnimMesh( "../_release/samples/mak/anim/makbot/mak_robotic.3DS" )
Expect bot_3ds<>0, "can load 3ds"
LoadAnimSeq bot_3ds,"../_release/samples/mak/anim/makbot/mak_running.3DS"

bot_x=LoadAnimMesh( "../_release/samples/mak/anim/makbot/mak_robotic.x")
Expect bot_x<>0, "can load x"
LoadAnimSeq bot_x,"../_release/samples/mak/anim/makbot/mak_running.x"

dragon=LoadMD2( "../_release/samples/mak/dragon/model/dragon.md2" );
Expect dragon<>0, "can load md2"
AnimateMD2 dragon,1,.05,0,40

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
RenderWorld

TrisRendered()

FreeEntity camera
FreeEntity plane
FreeEntity mirror
FreeEntity light
FreeEntity cube
