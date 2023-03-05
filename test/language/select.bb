
Context "Select"

; ints
abc = 123

Select abc
Case 000
	works_with_ints = false
Case 123
	works_with_ints = true
Default
	works_with_ints = false
End Select

Expect works_with_ints, "Selects work with ints"

; strings
hello$ = "world"

Select hello
Case "earth"
	works_with_strings = false
Case "world"
	works_with_strings = true
Default
	works_with_strings = false
End Select

Expect works_with_strings, "Selects work with strings"
