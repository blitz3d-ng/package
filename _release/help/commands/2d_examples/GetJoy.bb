; GetJoy Example

While Not KeyHit(1)
button=GetJoy()
If button <> 0 Then
Print "You pressed joystick button #" + button
End If
Wend
