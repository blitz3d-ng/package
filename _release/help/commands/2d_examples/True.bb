; TRUE example

; Assign test a random number of 0 or 1
test= Rnd(0,1)

; TRUE is implied; This statement REALLY means: if test=1 is TRUE then proceed
If test=1 Then
Print "Test was valued at 1"
End If

; Let's set test to be true
test=True

; Pointlessly test it
If test=True Then
Print "Test is true"
End If