; PlayCDTrack example

; Get a track to play from user
track=Input$("Enter a CD track number to play:")

; Play the track, assign a channel - just play once
chnCD=PlayCDTrack(track,1)

; Figure out what time it is now
oldTime=MilliSecs()
; Play until the channel is over or ESC
While ChannelPlaying(chnCD) And (Not KeyHit(1))
; clear and print the time elapsed
Cls 
Locate 0,0
Print "Time Elapsed (sec):" + ((MilliSecs()-oldTime)/1000)
Wend

; Stop the channel
StopChannel chnCD