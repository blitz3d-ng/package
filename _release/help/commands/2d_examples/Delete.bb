; Move them all over 1 (like the description example from TYPE command)
; If the chair isn't on the screen anymore, delete that chair object from the
; collection.

For room.chair = Each chair
room\x = room\x + 1
if room\x > 640 then
Delete room
Next