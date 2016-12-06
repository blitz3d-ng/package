; Log example

; NaN means "Not a Number", the numerical result is invalid.

; NOTE: All logarithm functions are related. For example, there is no
;       Log8 function in Blitz, but we can easily make one.

HidePointer

Print "Log8( -1 ) = " + Log8( -1 )
Print "Log8( 0  ) = " + Log8( 0 )
Print

; ... and now some well behaved numbers...

x# = 1.0 / 4096.0   ; small positive number

For n = 1 To 15

Print "Log8( " + LSet( x, 13 ) + " ) = " + Log8( x )
x = 8 * x

Next

WaitKey()  :  End


Function Log8# ( x# )

Return Log( x ) / Log( 8 )

End Function