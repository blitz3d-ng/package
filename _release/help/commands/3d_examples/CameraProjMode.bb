; CameraProjMode Example
; ----------------------

Graphics3D 640,480
SetBuffer BackBuffer()

camera=CreateCamera()
PositionEntity camera,0,0,-10

light=CreateLight()
RotateEntity light,0,0,0

; Create cube 1, near to camera
cube1=CreateCube()
EntityColor cube1,255,0,0
PositionEntity cube1,0,0,0

; Create cube 2, same size as cube 1 but further away
cube2=CreateCube()
EntityColor cube2,0,255,0
PositionEntity cube2,5,5,5

While Not KeyDown( 1 )

; If spacebar pressed then change mode value
If KeyHit(57)=True Then mode=mode+1 : If mode=3 Then mode=0

; If mode value = 2 (orthagraphic), then reduce zoom value to 0.1
If mode=2 Then zoom#=0.1 Else zoom#=1

; Set camera projection mode using mode value
CameraProjMode camera,mode

; Set camera zoom using zoom value
CameraZoom camera,zoom#

RenderWorld

Text 0,0,"Press spacebar to change the camera project mode"
Text 0,20,"CameraProjMode camera,"+mode
Text 0,40,"CameraZoom camera,"+zoom#

Flip

Cls

Wend

End