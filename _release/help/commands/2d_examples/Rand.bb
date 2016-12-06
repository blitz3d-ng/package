; Rand example

; Set the randomizer seed for more true random numbers
SeedRnd (MilliSecs())

; Generate random numbers between 1 and 100
For t = 1 To 20
Print Rand(1,100)
Next
