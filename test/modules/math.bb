
Context "Math"

Expect Floor(1.2) = 1, "Floor of 1.2 is 1"
Expect Ceil(1.2) = 2, "Ceil of 1.2 is 2"
Expect Sqr(49) = 7, "Square root of 49 is 7"

SeedRnd( 12345 )

Expect RndSeed() = 12345, "Random seed is 12345"

Expect Rand(0, 100) = 80, "First random number is 80"
Expect Rand(0, 100) = 96, "Second random number is 96"
Expect Rand(0, 100) = 20, "Third random number is 20"
Expect Rand(0, 100) = 75, "Fourth random number is 75"

Expect Sin(0) = 0, "Sine of 0 is 0"
;Expect Sin(90) = 1, "Sine of 90 is 1"

Expect Cos(0) = 1, "Cosine of 0 is 1"
;Expect Cos(90) = 0, "Cosine of 90 is 0"
