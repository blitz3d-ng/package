
Function FullName$(first_name$, last_name$)
	If first_name = "John"
		Return "..."
	EndIf

	Return first_name + " " + last_name
End Function

fname$ = "Kevin"
lname$ = "Primm"

Expect FullName(fname, lname) = "Kevin Primm", "full name should be Kevin Primm"
