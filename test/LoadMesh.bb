Graphics3D 1280,1024,32,2
SetBuffer BackBuffer()

camera=CreateCamera()

light=CreateLight()
RotateEntity light,90,0,0

; Load mesh
tank=LoadMesh("../_release/Media/geometricks_models/british-tadpole/btadpole.3ds")

PositionEntity tank,0,0,3
FitMesh tank,-1,-1,-1,2,2,2,True

While Not KeyDown( 1 )
	TurnEntity tank,1,1,0
	RenderWorld
	Flip
Wend

End
