; Channel examples

Print "Loading sound ..."
; Load the sample - you'll need to point this to a sound on your computer
; For best results, make it about 5-10 seconds...
sndWave=LoadSound("level1.wav")
; Prepare the sound for looping
LoopSound sndWave

chnWave=PlaySound(sndWave)

Print "Playing sound for 2 seconds ..."
Delay 2000

Print "Pausing sound for 2 seconds ..."
PauseChannel chnWave
Delay 2000

Print "Restarting sound ..."
ResumeChannel chnWave
Delay 2000

Print "Changing volume of sound ..."
ChannelVolume chnWave, .5
Delay 2000

Print "Playing new half-volume sound ..."
Delay 2000

Print "Left speaker only"
ChannelPan chnWave,-1
Delay 2000

Print "Right speaker only"
ChannelPan chnWave,1
Delay 2000

Print "All done!"
StopChannel chnWave
