; Float example
; =============

Print "Enter some text to be converted using Float()."
Print "Hit ENTER to exit."

Repeat
s$ = Input$(">")
If s$<>"" Then
Print "Float("+Chr$(34)+s$+Chr$(34)+")=" + Float(s$)
End If
Until s$=""

End