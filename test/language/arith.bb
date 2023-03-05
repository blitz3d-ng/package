
Context "Arithmetic"

a = 1
b = 2

Expect -b = -2, "-b = -2"

Expect a + b = 3, "1 + 2 = 3"
Expect a - b = -1, "1 - 2 = -1"

a = 2
b = 4

Expect a ^ b = 16, "2 ^ 4 = 16"

a = 20
Expect (a Mod 3) = 2, "20 Mod 3 = 2"

Expect True, "True = True"

cond = True
Expect Not cond = False, "Not True = False"

Expect +4 = 4, "a should equal 4"

eight = 8
Expect Abs(-eight) = eight, "absolute value of b should equal 8"

zero = 0
one  = 1
nine = 9
neg4 = -4
neg1 = -1
Expect Sgn(neg4) = neg1, "sgn of -4 is -1"
Expect Sgn(nine) = one, "sgn of 9 is 1"
Expect Sgn(zero) = zero, "sgn of 0 is 0"


a = 8
b = 2
Expect a Shl b = 32, "shift left works"
Expect a Shr b = 2, "shift right works"
;Expect -a Shr b = 1073741822, "shift right on a negative works" ; TODO: figure this out
Expect a Sar b = 2, "Sar works"
Expect -a Sar b = -2, "Negative Sar works"
