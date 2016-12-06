; TrisRendered Example
; --------------------

Graphics3D 640,480
SetBuffer BackBuffer()

camera=CreateCamera()
PositionEntity camera,0,0,-2

light=CreateLight()
RotateEntity light,90,0,0

segs=Rand( 2,16 )
sphere=CreateSphere(segs)

While Not KeyDown( 1 )

If KeyHit( 57 )=True

FreeEntity sphere
segs=Rand( 2,16 )
sphere=CreateSphere( segs )

EndIf

RenderWorld

Text 0,0,"Press space to create a sphere with a random segments value"

; Display triangles rendered
Text 0,20,"Triangles Rendered: "+TrisRendered()

Flip

Wend

End