
Type Game
	Field name$
	Field player.Player[5]
End Type

Type Player
	Field game.Game
	Field name$, x, y
End Type

Function EchoPlayer.Player(p.Player)
	; verifying casting of null ret
	return p
End Function

Expect First Game = Null, "No games"
Expect Last  Game = Null, "No games"

Expect Last  Player = Null, "No players"
Expect First Player = Null, "No players"

game.Game = New Game
game\name = "Demo"

p1.Player = New Player
p1\game = game
p1\name = "Kevin"
p1\x = 20
p1\y = 10
game\player[0] = p1

p2.Player = New Player
p2\game = game
p2\name = "Ryan"
p2\x = 40
p2\y = 23
game\player[1] = p2

p3.Player = New Player
p3\game = game
p3\name = "Trevor"
p3\x = 63
p3\y = 12
game\player[2] = p3

Expect game\player[0] = p1, "Game player 1 is p1"
Expect game\player[1] = p2, "Game player 2 is p2"
Expect game\player[2] = p3, "Game player 3 is p3"

Expect p1 <> Null, "Player 1 exists"
Expect p1\name = "Kevin", "Player 1 name = Kevin"
Expect p1\x = 20, "Player 1 x = 20"
Expect p1\y = 10, "Player 1 y = 10"
Expect p1\game\name = "Demo", "Player 1 game = Demo"

Expect p2 <> Null, "Player 2 exists"
Expect p2\name = "Ryan", "Player 2 name = Ryan"
Expect p2\x = 40, "Player 2 x = 40"
Expect p2\y = 23, "Player 2 y = 23"
Expect p2\game\name = "Demo", "Player 1 game = Demo"

Expect p3 <> Null, "Player 2 exists"
Expect p3\name = "Trevor", "Player 2 name = Trevor"
Expect p3\x = 63, "Player 2 x = 40"
Expect p3\y = 12, "Player 2 y = 23"
Expect p3\game\name = "Demo", "Player 1 game = Demo"

; players exprs...

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

; game exprs

Expect First Game = game, "game is first"
Expect Last Game = game, "game is last"

Expect After game = Null, "nothing after game"
Expect Before game = Null, "nothing before game"

count = 0
For g.Game = Each Game
	count = count + 1
Next

Expect count = 1, "There should be 1 games."

; for each exit test...

count = 0
For g.Game = Each Game
	Exit
Next

gamehandle = Handle(game)
g.Game = Object.Game(gamehandle)

Expect gamehandle > 0, "There is a handle."
Expect g <> Null, "The object has been recreated from the handle."
Expect g\name = "Demo" , "The recreated object has the right name."

Delete game
Delete Each Game
Expect First Game = Null, "No more games..."
