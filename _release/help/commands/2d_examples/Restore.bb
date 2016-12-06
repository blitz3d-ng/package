; Sample of read/restore/data/label commands

; Let's put the data pointer to the second data set
Restore seconddata

; Let's print them all to the screen
For t = 1 To 10
Read num ; Get the next data value in the data stack
Print num
Next

; Now for the first set of data
Restore firstdata

; Let's print them all to the screen
For t = 1 To 10
Read num ; Get the next data value in the data stack
Print num
Next

; this is the first set of data
.firstdata
Data 1,2,3,4,5,6,7,8,9,10

; this is the second set of data
.seconddata
Data 11,12,13,14,15,16,17,18,19,20