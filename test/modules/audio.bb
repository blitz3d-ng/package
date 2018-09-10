
Context "Audio"

boom = LoadSound( "_release/samples/mak/castle/sounds/boom.wav" )
Expect boom <> 0, "Boom sound was loaded"

channel = PlaySound( boom )
Expect channel <> 0, "Boom channel created"
Expect ChannelPlaying( channel ) = 1, "Boom channel playing"


channel = PlayMusic( "_release/samples/mak/castle/sounds/shoot.wav" )
Expect channel <> 0, "Streaming shoot sound"

Delay 1000
