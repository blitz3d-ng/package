; Log10 example

; NaN means "Not a Number", the numerical result is invalid.

HidePointer

; First, what happens with non-positive numbers...

Print "Log10( -1 ) = " + Log10( -1 )
Print "Log10( 0  ) = " + Log10( 0 )
Print

; ... and now some well behaved numbers...

x# = 1.0 / 4096.0   ; small positive number

For n = 1 To 15

Print "Log10( " + LSet( x, 13 ) + " ) = " + Log10( x )
x = 8 * x

Next

WaitKey()  :  End