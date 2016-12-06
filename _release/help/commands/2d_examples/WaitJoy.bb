; WaitJoy() sample

Print "Press a joystick button to continue."

button=WaitJoy()

Print "The joystick button code of the button you pressed was: " + button
Print "Now press a button to quit."

WaitJoy()

End
