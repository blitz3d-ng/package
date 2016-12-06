; Sin / Cos / Tan example.

; Left/Right arrow keys change angle. Escape quits.

Const width = 640, height = 480
Const radius# = .2 * height
Const KEY_ESC = 1, KEY_LEFT = 203, KEY_RIGHT = 205 

Graphics width, height
SetBuffer BackBuffer( )
Origin width / 3, height / 2

angle# = 0.0

While Not KeyDown( KEY_ESC )

; NOTE: It is usually best to avoid very large angles.
;       The 'If angle...' lines show one way to do this.
;		Mod is another possibility. 

If KeyDown( KEY_LEFT ) Then angle = angle - .5
;	If angle < 0.0 Then angle = angle + 360

If KeyDown( KEY_RIGHT ) Then angle = angle + .5
;	If angle >= 360.0 Then angle = angle - 360

Cls

Color 80, 80, 0		; pale yellow circle
Oval -radius, -radius, 2 * radius, 2 * radius, False

For a# = 0.0 To Abs( angle Mod 360 ) Step .5

x# = radius * Cos( a )	; (x,y) is a point on the circle
y# = radius * Sin( a )	; corresponding to angle a.

If ( angle Mod 360 < 0 ) Then y = -y	; reverse for negative angle		
WritePixel x, y, $ffff00		; bright yellow

Next

Color 255, 255, 0		; yellow
Line 0, 0, radius * Cos( angle ), radius * Sin( angle ) 

Color 0, 255, 0			; green
Line 0, 0, radius * Cos( angle ), 0
Text radius * 1.5,  10, "Cos( angle ) = " + Cos( angle )

Color 255, 0, 0			; red
Line radius * Cos( angle ), 0, radius * Cos( angle ), radius * Sin( angle ) 	
Text radius * 1.5, -10, "Sin( angle ) = " + Sin( angle )

Color 255, 255, 255
Text radius * 1.5, -30, "     angle =   " + angle
Text radius * 1.5,  30, "Tan( angle ) = " + Tan( angle ) 

Flip

Wend

End