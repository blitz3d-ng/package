
Context "Select"

works_with_strings = false
works_with_ints = false

hello = "world"

Select hello
Case "world"
	works_with_strings = true
End Select

abc = 123

Select abc
Case 123
	works_with_ints = true
End Select

Expect works_with_strings, "Selects work with strings"
Expect works_with_ints, "Selects work with ints"
