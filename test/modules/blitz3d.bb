
Context "Blitz3D"

;light = CreateLight()
;Expect light <> 0, "Can create a light"

pivot = CreatePivot()
Expect pivot <> 0, "Can create a pivot"
FreeEntity pivot

;camera = CreateCamera()
;Expect camera <> 0, "Can create a camera"

cube = CreateCube()
Expect cube <> 0, "Can create a cube"
Expect MeshWidth(cube) = 2, "Cube has a width of 2"
Expect MeshHeight(cube) = 2, "Cube has a height of 2"
Expect MeshDepth(cube) = 2, "Cube has a depth of 2"
Expect CountSurfaces(cube) = 1, "Cube has one surface"

FreeEntity cube
