; EntityClass Example
; -------------------

Graphics3D 640,480,16
SetBuffer BackBuffer()

SeedRnd MilliSecs()

; Select a random number between 0 and 7 then create a certain class of entity depending on the number selected
i=Rand(0,7)
Select i
	Case 0 ent=CreatePivot()
	Case 1 ent=CreateLight()
	Case 2 ent=CreateCamera()
	Case 3 ent=CreateMirror()
	Case 4 ent=CreateSprite()
	Case 5 ent=CreateTerrain(32)
	Case 6 ent=CreatePlane()
	Case 7 ent=CreateMesh()
	;Case 8 ent=CreateListener(parent)
	;Case 9 ent=LoadMD2(md2_file$)
	;Case 10 ent=LoadBSP(bsp_file$)
End Select

; Get the class of the entity
class$=EntityClass$(ent)

; Output the class to the screen
Text 0,0,"A "+class$+" was created."

Text 0,20,"Press a key."

WaitKey()

End