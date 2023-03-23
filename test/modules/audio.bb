
Context "Audio"

boom = LoadSound( "../_release/samples/mak/castle/sounds/boom.wav" )
Expect boom <> 0, "Boom sound was loaded"

sound = PlaySound( boom )
Expect sound <> 0, "Boom channel created"
Expect ChannelPlaying( sound ) = 1, "Boom channel playing"

music = PlayMusic( "../_release/samples/mak/castle/sounds/shoot.wav" )
Expect music <> 0, "Streaming shoot sound"

While ChannelPlaying( sound ) Or ChannelPlaying( music )
	Delay 100
Wend

FreeSound boom
