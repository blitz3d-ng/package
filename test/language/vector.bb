
Local temperatures[5]

Function SetTemperatures( temps[5] )
	temps[0] = 72
	temps[1] = 40
	temps[2] = 55
	temps[3] = 68
	temps[4] = 50
End Function

SetTemperatures( temperatures )

Expect temperatures[0] = 72, "temp 1 is 72"
Expect temperatures[1] = 40, "temp 2 is 40"
Expect temperatures[2] = 55, "temp 3 is 55"
Expect temperatures[3] = 68, "temp 4 is 68"
Expect temperatures[4] = 50, "temp 5 is 50"

i = 2
Expect temperatures[i] = 55, "temp 3 is 55"
