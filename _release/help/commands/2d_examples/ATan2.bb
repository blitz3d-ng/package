; ATan2 example.

; Move mouse. Escape quits.

Const width = 640, height = 480
Const radius# = .2 * height
Const KEY_ESC = 1

Graphics width, height
SetBuffer BackBuffer( )
Origin width / 2, height / 2
HidePointer 
MoveMouse .75 * width, height / 2

While Not KeyDown( KEY_ESC )

Cls

Color 255, 255, 0
Line 0, 0, width / 2, 0   ; positive x-axis

x = MouseX() -  width / 2
y = MouseY() - height / 2

Oval x - 3, y - 3, 7, 7, True
Line 0, 0, x, y

Text .35 * width, -80, "x = " + x
Text .35 * width, -60, "y = " + y

Text .35 * width - 96, -40, "ATan2( y, x ) = " + ATan2( y, x )

Flip

Wend

End