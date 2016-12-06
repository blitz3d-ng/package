; Assign a global variable for the sound loop
Global sndMusicLoop

; Load the sound loop file into memory

sndMusicLoop=LoadSound("sounds/loop1.wav")

; Set the sound loop

LoopSound sndMusicLoop

PlaySound sndMusicLoop
