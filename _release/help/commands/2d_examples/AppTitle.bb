; AppTitle
; --------

AppTitle "Super Invaders V1.0", "Are you sure?"

Graphics 800,600,32,2
SetBuffer BackBuffer()

While Not KeyDown(1)
	Cls
	Text 0,0,"Press the X button to close."
	Flip
Wend

End
