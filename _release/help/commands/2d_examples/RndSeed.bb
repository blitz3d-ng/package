;'randomize' the random number generator!
SeedRnd MilliSecs()
For k=1 To 12345
Rnd(1)
Next

;capture random number generator state
state=RndSeed()

;generate a bunch of numbers:
Print "First set:"
For k=1 To 5
Print Rnd(1)
Next

;restore random nummber generator state
SeedRnd state

;generate another bunch:
Print "Second set (same as the first set...):"
For k=1 To 5
Print Rnd(1)
Next