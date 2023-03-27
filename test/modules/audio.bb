
Context "Audio"

snap_wav = LoadSound( "media/snap.wav" )
Expect snap_wav <> 0, "WAV was loaded"

channel_wav = PlaySound( snap_wav )
Expect channel_wav <> 0, "WAV channel created"
Expect ChannelPlaying( channel_wav ) = 1, "WAV channel playing"

Delay 100

snap_ogg = LoadSound( "media/snap.ogg" )
Expect snap_ogg <> 0, "OGG was loaded"

channel_ogg = PlaySound( snap_ogg )
Expect channel_ogg <> 0, "OGG channel created"
Expect ChannelPlaying( channel_ogg ) = 1, "OGG channel playing"

music = PlayMusic( "../_release/samples/mak/castle/sounds/shoot.wav" )
Expect music <> 0, "Streaming shoot sound"

While ChannelPlaying( channel_wav ) Or ChannelPlaying( channel_ogg ) Or ChannelPlaying( music )
	Delay 10
Wend

FreeSound boom
