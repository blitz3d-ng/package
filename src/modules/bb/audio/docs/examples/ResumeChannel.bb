Graphics 640, 480, 0, 2

musicchannel = PlayMusic ("oohyeahbaby.mp3") ; Replace with a music file on  your hard drive!

Repeat

    Print "Press a key to pause the music..."
    WaitKey

    PauseChannel musicchannel

    Print "Press a key to continue the music..."
    WaitKey

    ResumeChannel musicchannel

Until KeyHit (1)

End
