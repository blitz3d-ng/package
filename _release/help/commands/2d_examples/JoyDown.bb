; JoyDown Example

; Until user presses ESC, show the mouse button pressed
While Not KeyHit(1)
button$="No"
For t = 1 To 5
If JoyDown(t) Then button$=Str(t)
Print button$ + " joystick button pressed!"
Next
Wend
