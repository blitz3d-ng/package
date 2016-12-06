num=%11110000111100001111000011110000 ; Define a bit pattern which is easy  to recognize

bitmask=Rnd(-2147483648,2147483647) ; Define a RANDOM Xor 32bit wide bitmask

; This line prints the binary and decimal numbers before the Xor
Print "Binary number is: "+Bin$(num)+" ("+num+")"

; This line prints the binary and decimal numbers of the Xor bitmask
Print "Xor bitmask is: "+Bin$(bitmask)+" ("+bitmask+")"

Print "------------------------------------------------------------------"

; This line Xor's the number with the bitmask
xres=num Xor bitmask

; This line prints the binary and decimal numbers after the Xor
Print "Xor result is: "+Bin$(xres)+" ("+xres+")"
Print "------------------------------------------------------------------"

; This line Xor's the Xor result with the bitmask again
xres=xres Xor bitmask
; This line prints the binary and decimal numbers after the second Xor. NOTE:  This number is identical to the original number
Print "Result Xor'ed again: "+Bin$(xres)+" ("+xres+")"

WaitMouse ; Wait for the mouse before ending.