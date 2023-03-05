
Context "Loops"

a = 1
While a < 10
	a = a + 1
Wend

Expect a = 10, "a should be 10"

;------

t = 0
While True
	t = t + 4

	If t = 64 Then Exit
Wend

Expect t = 64, "t should be 64"

;------

countdown = 100
Repeat
	countdown = countdown - 1
Until countdown = 0

Expect countdown = 0, "countdown should have been reached"

;---

h = 123
Repeat
	h = 456
	Exit ; JK, nothing lasts forever.
Forever

Expect h = 456, "entered repeat/forever and then bailed"

;------

count = 0
For i = 0 To 99
	count = count + 1
Next

Expect count = 100, "should have iterated 100 times, counting up"

;------

count = 100
For i = 99 To 0 Step -1
	count = count - 1
Next

Expect count = 0, "should have iterated 100 times, counting down"

;------

count = 0
For i = 0 To 99
	count = count + 1
	If count = 50
		Exit
	EndIf
Next

Expect count = 50, "should have iterated 50 times, exiting early"
