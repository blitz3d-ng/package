; JoyType() example

; Check to see what stick is present - print the proper message
Select JoyType()
Case 0
Print "Sorry, no joystick attached to system!"
Case 1
Print "Digital joystick is attached to system!"
Case 2
Print "Analog joystick is attched to system!"
End Select

; Wait for user to hit ESC
While Not KeyHit(1)
Wend
