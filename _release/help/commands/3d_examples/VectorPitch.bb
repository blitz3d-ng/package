; VectorPitch Example
; -------------------

Graphics3D 640,480
SetBuffer BackBuffer()

; Set vector x,y,z values
vx#=2
vy#=2
vz#=0

camera=CreateCamera()
PositionEntity camera,0,0,-5

light=CreateLight()

cone=CreateCone()

 ; Rotate cone so that it points in the direction it is facing
RotateMesh cone,90,0,0

pivot=CreatePivot()

; Position pivot to represent a vector
PositionEntity pivot,vx#,vy#,vz#

; Point cone in direction of pivot. It should now have same pitch value.
PointEntity cone,pivot

While Not KeyDown(1)

	RenderWorld
	
	; Print entity pitch value and vector pitch value. Both should be identical.
	Text 0,0,"Entity pitch: "+EntityPitch(cone)
	Text 0,20,"Vector pitch: "+VectorPitch(vx#,vy#,vz#)
	
	Flip

Wend

End