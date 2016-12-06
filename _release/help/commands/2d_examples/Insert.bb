; INSERT example

; Define a CHAIR type with a created field to track what order it was created  in.
Type CHAIR
Field created
End Type 

; Create 10 chairs, setting created field to the order of creation
For t = 1 To 10
room.chair= New Chair
room\created = t
Next

; Make a NEW chair (the 11th) 
room.chair= New Chair

; Set its created value to 11
room\created=11

; Now, let's insert this chair BEFORE the first one in the collection
Insert room Before First Chair

; Let's iterate through all the chairs, and show their creation order
For room.chair = Each chair
Print room\created
Next