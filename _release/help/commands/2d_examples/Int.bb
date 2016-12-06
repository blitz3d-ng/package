; Ceil / Floor / Int example, three kinds of rounding.

; Move mouse. Escape quits.

Graphics 640, 480

Const KEY_ESC = 1

SetBuffer BackBuffer()
Origin 320, 240

MoveMouse 320, 240  :  HidePointer

While Not KeyDown( KEY_ESC )

Cls

my = MouseY() - 240
Color 100, 100, 0
Line -320, my, 319, my

DrawNumberLine

y# = Float( -my ) / 32

Text 100, 50, "          y = "  + y
Text 100, 70, "  Ceil( y ) = "  + Ceil( y )
Text 100, 90, " Floor( y ) = "  + Floor( y )
Text 100, 110, "   Int( y ) = " + Int( y )

Flip

Wend
End

Function DrawNumberLine( )  ; vertical line with numeric labels

Color 255, 255, 255
Line 0, -240, 0, 239

For n = -7 To 7
yn = -32 * n
Line -2, yn, 2, yn
Text -30, yn - 6, RSet( n, 2 )
Next

End Function