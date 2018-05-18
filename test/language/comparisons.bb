
Context "Comparisons"

Expect 2 > 1, "2 should be greater than 1"
Expect 2 >= 2, "2 should be greater than or equal to 2"
Expect 5 < 10, "5 should be less than 10"
Expect 3 = 3, "3 should be equal 3"

str1$ = "hello"
str2$ = "hello"
Expect str1 = str2, "string should match"
