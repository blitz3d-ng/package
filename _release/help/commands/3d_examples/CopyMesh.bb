; CopyMesh Example
; ----------------

Graphics3D 640,480
SetBuffer BackBuffer()

camera=CreateCamera()

light=CreateLight()
RotateEntity light,90,0,0

crate1=LoadMesh("media/wood-crate/wcrate1.3ds")
PositionEntity crate1,-40,0,100

crate2=CopyMesh(crate1)
PositionEntity crate2,40,0,100

While Not KeyDown(1)

TurnEntity crate1,1,1,1
TurnEntity crate2,1,1,-1

RenderWorld
Flip

Wend

End