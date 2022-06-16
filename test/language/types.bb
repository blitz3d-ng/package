
Type Game
	Field name$
End Type

Type Player
	Field name$, x, y
End Type

game.Game = New Game

p1.Player = New Player
p1\name = "Kevin"
p1\x = 20
p1\y = 10

p2.Player = New Player
p2\name = "Ryan"
p2\x = 40
p2\y = 23

p3.Player = New Player
p3\name = "Trevor"
p3\x = 63
p3\y = 12

Expect p1 <> Null, "Player 1 exists"
Expect p1\name = "Kevin", "Player 1 name = Kevin"
Expect p1\x = 20, "Player 1 x = 20"
Expect p1\y = 10, "Player 1 y = 10"

Expect p2 <> Null, "Player 2 exists"
Expect p2\name = "Ryan", "Player 2 name = Ryan"
Expect p2\x = 40, "Player 2 x = 40"
Expect p2\y = 23, "Player 2 y = 23"

Expect p3 <> Null, "Player 2 exists"
Expect p3\name = "Trevor", "Player 2 name = Trevor"
Expect p3\x = 63, "Player 2 x = 40"
Expect p3\y = 12, "Player 2 y = 23"

Expect First Player = p1, "p1 is first"
Expect Last Player = p3, "p3 is last"

Expect After p1 = p2, "p2 is after p1"
Expect Before p3 = p2, "p2 is before p2"

Insert p3 Before p1
Insert p2 After p1
Expect First Player = p3, "p3 is now first"
Expect Last Player = p2, "p3 is now last"

count = 0
For p.Player = Each Player
	count = count + 1
Next

Expect count = 3, "There should be 3 players."

p1handle = Handle(p1)
p1object.Player = Object.Player(p1handle)

Expect p1handle > 0, "There is a handle."
Expect p1object <> Null, "The object has been recreated from the handle."
Expect p1object\name = "Kevin" , "The recreated object has the right name."

Delete p3
Expect First Player = p1, "p1 is now first"
Expect Last Player = p2, "p2 is now last"

Delete Each Player
Expect First Player = Null, "No more players..."
