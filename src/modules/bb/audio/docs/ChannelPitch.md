# ChannelPitch channel, pitch

## Parameters

- channel = variable assigned to the channel when played
- pitch = pitch, in hertz, to apply to the channel (try 8000-44000)

## Description

You can alter the pitch of a sound channel that is playing (or in use and just paused). I'm sure you can think of numerous uses for this command!  Use the frequency of your sound as the 'baseline' for pitch change. So if your  sample is at 11025 hertz, increase the pitch to 22050 to make the pitch twice as high, 8000 to make it lower, etc. While similar to [SoundPitch](#SoundPitch),  this command let's you change the pitch individually of each and every channel  in use.
