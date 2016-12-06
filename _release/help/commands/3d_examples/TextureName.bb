; TextureName Example
; -------------------

Graphics3D 640,480
SetBuffer BackBuffer()

camera=CreateCamera()

light=CreateLight()
RotateEntity light,90,0,0

; Load mesh
crate=LoadMesh("media/wood-crate/wcrate1.3ds")
PositionEntity crate,0,0,100

; Get mesh surface
surf=GetSurface(crate,1)

; Get surface brush
crate_brush=GetSurfaceBrush(surf)

; Get brush texture
crate_tex=GetBrushTexture(crate_brush,0)

While Not KeyDown( 1 )

	RenderWorld

	; Display full texture name
	Text 0,0,"Texture name, as returned by TextureName$():"
	Text 0,20,TextureName$(crate_tex)
	
	; Display trimmed texture name
	Text 0,40,"Texture name with path stripped:"
	Text 0,60,StripPath$(TextureName$(crate_tex))

	Flip

Wend

End

Function StripPath$(file$) 

	If Len(file$)>0 
	
		For i=Len(file$) To 1 Step -1 
		
			mi$=Mid$(file$,i,1) 
			If mi$="\" Or mi$="/" Then Return name$ Else name$=mi$+name$ 
		
		Next 
	
	EndIf 
	
	Return name$ 

End Function