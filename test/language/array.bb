
Context "Arrays"

Dim tiles(10,10)

tiles(0,0) = 1
tiles(1,0) = 2
tiles(1,1) = 3

Expect tiles(0,0) = 1, "array index 0,0 is 1"
Expect tiles(1,0) = 2, "array index 1,0 is 2"
Expect tiles(1,1) = 3, "array index 1,1 is 3"
