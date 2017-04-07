; EntityFX Example
; ----------------

Graphics3D 640,480
SetBuffer BackBuffer()

camera=CreateCamera()

light=CreateLight()
RotateEntity light,90,0,0

; Create cube
cube=CreateCube()
EntityColor cube,0,0,255
PositionEntity cube,-2,0,5

wire_cube=CreateCube()
EntityColor wire_cube,255,0,0
EntityFX wire_cube,64
PositionEntity wire_cube,2,0,5

While Not KeyDown( 1 )
	; If you press spacebar, both cubes are wireframe.
	If KeyHit( 57 )=True Then enable=Not enable 
	WireFrame enable
	
	TurnEntity cube,0.5,0.5,0
	TurnEntity wire_cube,0.5,0.5,0
	RenderWorld
	Text 0,0,"Press spacebar to render both cubes as wireframes."
	Flip
Wend

End