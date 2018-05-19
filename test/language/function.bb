
Function FullName$(first_name$, last_name$)
	Return first_name + " " + last_name
End Function

fname$ = "Kevin"
lname$ = "Primm"

Expect FullName(fname, lname) = "Kevin Primm", "full name should be Kevin Primm"
