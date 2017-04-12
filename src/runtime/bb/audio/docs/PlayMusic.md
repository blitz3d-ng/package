# PlayMusic (filename$)

## Parameters

- filename$ - name of music file. Formats supported:  raw/mod/s3m/xm/it/mid/rmi/wav/mp2/mp3/ogg/wma/asf

## Description

This command will load and play a music file.You MUST use a channel variable  in order to stop or adjust the music playing. You may use StopChannel, PauseChannel, ResumeChannel, etc. with this command.
You can't 'preload' the audio like you can a sound sample via the LoadSound command. Every time you call the PlayMusic  command, the file is reloaded and played. This means that if you use the command  while some graphics are moving on-screen, you may get a slight pause when the  hard drive seeks and grabs the music file. To avoid this, you might want to  use the PlaySound/LoopSound  commands instead.
