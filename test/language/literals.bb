
Context "Literals"

fourOhNineFive = $fff
Expect fourOhNineFive = 4095, "$fff = 4095"

sos = %1001001
Expect sos = 73, "1001001 = 73"

oneHalf# = .5 ; the lack of a zero here is intentional
Expect oneHalf < 1 And oneHalf > 0, ".5 is between 0 and 1"
