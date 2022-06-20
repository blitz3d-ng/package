
Context "Arrays"

Dim list(5)
list(0) = 1
list(1) = 2
list(2) = 3
list(3) = 4
list(4) = 5
list(5) = 6

Expect list(0) = 1, "list index 0 is 1"
Expect list(1) = 2, "list index 1 is 2"
Expect list(2) = 3, "list index 2 is 3"
Expect list(3) = 4, "list index 3 is 4"
Expect list(4) = 5, "list index 4 is 5"
Expect list(5) = 6, "list index 5 is 6"

Dim tiles(5,5)

tiles(1,1) = 1
tiles(2,1) = 2
tiles(1,2) = 3
tiles(2,2) = 4

Expect tiles(1,1) = 1, "array index 1,1 is 1"
Expect tiles(2,1) = 2, "array index 2,1 is 2"
Expect tiles(1,2) = 3, "array index 1,2 is 3"
Expect tiles(2,2) = 4, "array index 2,2 is 4"

Type Level
	Field name$
End Type

Dim levels.Level(10)

i = 4
levels(i) = New Level
levels(i)\name = "Intro"

Expect levels(i)\name = "Intro", "Level 1 is Intro"
