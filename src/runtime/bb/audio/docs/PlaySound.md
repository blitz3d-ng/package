# PlaySound ( sound )

## Parameters

- sound = variable previously assigned with a LoadSound command.

## Description

This plays a sound previously loaded and assigned to a variable using the LoadSound command. See example.
You will need to assign a channel variable handle to the sound when you play  it. All subsequent sound handling commands require you use the CHANNEL variable,  not the sound variable to control the sound - such as StopChannel, PauseChannel, ResumeChannel, ChannelPitch, ChannelVolume, ChannelPan,  and ChannelPlaying.