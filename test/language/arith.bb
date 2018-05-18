
Context "Arithmetic"

a = 1
b = 2

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
