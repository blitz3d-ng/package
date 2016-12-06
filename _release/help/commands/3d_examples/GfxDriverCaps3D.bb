Graphics3D 640,480,0,2

caps=GfxDriverCaps3D()

Select caps

	Case 100 msg$="Your graphics card does not support cubic environment mapping."

	Case 110 msg$="Your graphics card does support cubic environment mapping."

End Select

Print msg$

WaitKey()