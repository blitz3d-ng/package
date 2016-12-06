; Bank Commands Example
; ---------------------

bnkTest=CreateBank(12)

PokeByte bnkTest,0,Rand(255)
PokeShort bnkTest,1,Rand(65535)
PokeInt bnkTest,3,Rand(-2147483648,2147483647)
PokeFloat bnkTest,7,0.5

Print PeekByte(bnkTest,0)
Print PeekShort(bnkTest,1)
Print PeekInt(bnkTest,3)
Pint PeekFloat(bnkTest,7)

FreeBank bnkTest