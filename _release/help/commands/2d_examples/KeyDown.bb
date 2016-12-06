; KeyDown() example

Print "Hold down ENTER key!"
Delay 3000
While Not KeyHit(1)
If KeyDown(28) Then 
Print "Enter is being pressed!"
Else
Print 
End If
Wend