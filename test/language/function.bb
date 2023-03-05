
Function FullName$(first_name$, last_name$)
	; do some nonsense things to trigger various codegen branches...
	If first_name = "John"
		first_name = 3
		Return "..."
	EndIf

	Goto skip
	Return
	.skip

	Return first_name + " " + last_name
End Function

fname$ = "Kevin"
lname$ = "Primm"

Expect FullName(fname, lname) = "Kevin Primm", "full name should be Kevin Primm"
