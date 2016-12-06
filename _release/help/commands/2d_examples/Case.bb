; Advanced SELECT/CASE/DEFAULT/END SELECT Example
; Assign a random number 1-10
mission=Rnd(1,10)

; Start the selection process based on the value of 'mission' variable
Select True

; Is mission = 1?
Case mission=1
Print "Your mission is to get the plutonium and get out alive!"

; Is mission = 2?
Case mission=2
Print "Your mission is to destroy all enemies!"

; Is mission = 3 to 5
Case mission>=3 And mission<=5
Print "Your mission is to steal the enemy building plans!"

; What do do if none of the cases match the value of mission
Default 
Print "Missions 6-10 are not available yet!"

; End the selection process
End Select