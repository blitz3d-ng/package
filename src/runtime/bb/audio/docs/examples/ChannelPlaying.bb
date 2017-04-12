; Channel examples

Print "Loading sound ..."
; Load the sample - you'll need to point this to a sound on your computer
; For best results, make it about 5-10 seconds...
sndWave=LoadSound("level1.wav")

Print "Playing full sample until sound is done ..."
chnWave=PlaySound(sndWave) 
While ChannelPlaying(chnWave)
Wend 
Print "All done!"
