; GetMouse Example

While Not KeyHit(1)
button=GetMouse()
If button <> 0 Then
Print "You pressed mouse button #" + button
End If
Wend
