; HidePointer / ShoPointer Example

; draw a simple screen, cut in half by a white line
Graphics 800,600,0,2
Color 255,255,255
Line 400,0,400,600
Text 200,300,"ShowPointer",True,True
Text 600,300,"HidePointer",True,True

; and a simple loop in which we hide / show the pointer dependent on 
; which side of the screen the mouse is on!
Repeat
	If MouseX()<400 Then 
		ShowPointer
	Else
		HidePointer
	End If
	If KeyHit(1) Then Exit ; ESCAPE to exit
Forever

End ; bye!