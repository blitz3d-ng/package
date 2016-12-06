; MouseDown Example

; Until user presses ESC, show the mouse button pressed
While Not KeyHit(1)
button$="No"
If MouseDown(1) Then button$="Left"
If MouseDown(2) Then button$="Right"
If MouseDown(3) Then button$="Middle"

Print button$ + " mouse button pressed!"
Wend
