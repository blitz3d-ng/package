; Define the CHAIR Type

Type CHAIR
Field X
Field Y
Field HEIGHT
End Type

; Create 100 new chairs using FOR ... NEXT using the collection name of ROOM 

For tempx = 1 to 10
For tempy = 1 to 10
room.chair = New Chair
room\x = tempx
room\y = tempy
room\height = Rnd(0,10) ; set a random height 0 to 10
Next
Next 

; Move them all over 1 (like the description example from TYPE command) 

For room.chair = Each chair
room\x = room\x + 1
Next