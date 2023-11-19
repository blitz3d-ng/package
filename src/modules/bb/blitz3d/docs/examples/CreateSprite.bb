Graphics3D 1280,720,32,2

cam = CreateCamera()
MoveEntity cam,0,0,-5

sp = CreateSprite()
RotateSprite sp,20

RenderWorld:Flip
WaitKey
End
