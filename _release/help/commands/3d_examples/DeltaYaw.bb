SetBuffer BackBuffer()

camera=CreateCamera()

; Make camera orthagraphic for flat, 2D view
CameraProjMode camera,2

; Position and rotate camera so we have overhead (top-down) view
PositionEntity camera,0,5,0
RotateEntity camera,90,0,0

; Create red cone (the arrow)
arrow=CreateCone()
RotateMesh arrow,90,180,0
ScaleMesh arrow,.1,.1,.2
EntityColor arrow,255,0,0

; Create blue sphere (the spot)
spot=CreateSphere()
ScaleMesh spot,.1,.1,.1
EntityColor spot,0,0,255

While Not KeyDown(1)

	; If w,a,s,d pressed then move spot
	If KeyDown(17)=True Then MoveEntity spot,0,0,0.01 ; w - up
	If KeyDown(30)=True Then MoveEntity spot,-0.01,0,0 ; a - left
	If KeyDown(31)=True Then MoveEntity spot,0,0,-0.01 ; s - down
	If KeyDown(32)=True Then MoveEntity spot,0.01,0,0 ; d - right

	; Rotate arrow using delta yaw value. Arrow will point at spot.
	RotateEntity arrow,0,DeltaYaw#(spot,arrow),0

	RenderWorld

	Text 0,0,"Note: Camera view is overhead. The arrow will y-rotate using DeltaYaw value."
	Text 0,20,"Use w,a,s,d to move spot."
	Text 0,40,"Delta yaw: "+DeltaYaw#(spot,arrow)

	Flip
	Wend

End