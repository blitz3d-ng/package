Graphics3D(800,600)

light = CreateLight()

cam = CreateCamera()

cube = CreateCube()
PositionEntity(cube,0,0,5)

while KeyDown(1) == 0 do
  TurnEntity(cube,1,1,0)
  RenderWorld()
  Flip()
end
