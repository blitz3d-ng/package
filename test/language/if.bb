
Context "If"

a = 5
b = 2

If a > b
	c = 3
ElseIf a < b
	c = 2
Else
	c = 1
End If

Expect c = 3, "c should be 3"
