
Local local_temperatures[5]
Global global_temperatures[5]

Function SetTemperatures( temps[5] )
	temps[0] = 72
	temps[1] = 40
	temps[2] = 55
	temps[3] = 68
	temps[4] = 50

	; confirm globals work
	global_temperatures[2] = 45
End Function

SetTemperatures( local_temperatures )
SetTemperatures( global_temperatures )

WriteStdout "result: "+global_temperatures[0]

Expect global_temperatures[0] = 72, "temp 1 is 72"
Expect global_temperatures[1] = 40, "temp 2 is 40"
Expect global_temperatures[2] = 45, "temp 3 is 45"
Expect global_temperatures[3] = 68, "temp 4 is 68"
Expect global_temperatures[4] = 50, "temp 5 is 50"

Expect local_temperatures[3] = 68, "temp 3 is 68"

i = 2
Expect global_temperatures[i] = 45, "temp 3 is 45"
