; AddMesh Example
; ---------------

Graphics3D 640,480
SetBuffer BackBuffer()

camera=CreateCamera()
PositionEntity camera,0,0,-10

light=CreateLight()
RotateEntity light,90,0,0

; Create tree mesh (upper half)
tree=CreateCone()
green_br=CreateBrush(0,255,0)
PaintMesh tree,green_br
ScaleMesh tree,2,2,2
PositionMesh tree,0,1.5,0

; Create trunk mesh
trunk=CreateCylinder()
brown_br=CreateBrush(128,64,0)
PaintMesh trunk,brown_br
PositionMesh trunk,0,-1.5,0

; Add trunk mesh to tree mesh, to form one whole tree
AddMesh trunk,tree

; Free trunk mesh - we don't need it anymore
FreeEntity trunk

While Not KeyDown( 1 )

TurnEntity tree,1,1,1

RenderWorld
Flip

Wend

End