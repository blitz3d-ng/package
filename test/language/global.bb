
Global AGlobalValue = 1234

Function ChangeTheValue()
  ALocalValue = 10000000
  AGlobalValue = 5678
End Function

Expect AGlobalValue = 1234, "AGlobalValue is initally 1234"
Expect ALocalValue = 0, "ALocalValue is 0"

ChangeTheValue()

Expect AGlobalValue = 5678, "AGlobalValue is now 5678"
Expect ALocalValue = 0, "ALocalValue is still 0"
