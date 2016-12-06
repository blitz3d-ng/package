Print "Here we go!"

; Restore to the start of the Data statements
Restore startData

; Get the first number which is the total number of users
Read Users

; Print them all!
For T = 1 To Users
Read firstname$
Read age
Read accuracy#
Read lastname$
Print firstname$ + " " + lastname$ + " is " + age + " years old with " + accuracy#  + " accuracy!"
Next 

While Not KeyHit(1)
Wend
End

.startData
Data 3
Data "Shane", 31, 33.3333, "Monroe"
Data "Bob", 28, 12.25, "Smith"
Data "Roger", 54, 66.66, "Rabbit"