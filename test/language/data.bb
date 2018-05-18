
Context "Data"

Restore startData

; Get the first number which is the total number of users
Read NumberOfUsers
Expect NumberOfUsers = 3, "there should be three users"

Read firstname$, age, accuracy#, lastname$
Expect firstname = "Shane", "firstname should be Shane"
Expect age = 31, "age should be 31"
Expect accuracy > 33 And accuracy < 34, "accuracy should be above 33 and below 34"
Expect lastname = "Monroe", "last name should be Monroe"

Read firstname$, age, accuracy#, lastname$
Expect firstname = "Bob", "firstname should be Bob"
Expect age = 28, "age should be 31"
Expect accuracy > 12 And accuracy < 13, "accuracy should be above 33 and below 34"
Expect lastname = "Smith", "last name should be Smith"

Read firstname$, age, accuracy#, lastname$
Expect firstname = "Roger", "firstname should be Roger"
Expect age = 54, "age should be 31"
Expect accuracy > 66 And accuracy < 67, "accuracy should be above 12 and below 13"
Expect lastname = "Rabbit", "last name should be Rabbit"

.startData
Data 3
Data "Shane", 31, 33.3333, "Monroe"
Data "Bob", 28, 12.25, "Smith"
Data "Roger", 54, 66.66, "Rabbit"
