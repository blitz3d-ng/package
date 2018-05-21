; Load the sound (11,000 hertz)

snd1Up = LoadSound("audiooneup.wav")

; Play the sound normally
PlaySound snd1Up

; Change the pitch UP and play it
SoundPitch snd1Up, 11000*2 ;twice original frequency
PlaySound snd1Up

; Change the pitch down and play it
SoundPitch snd1Up, 11000/2 ;1/2 original frequency
PlaySound snd1Up