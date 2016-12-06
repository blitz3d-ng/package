; JoyU Example
; ------------

While Not KeyDown(1)

; Get various joystick values
ju#=JoyU()
judir=JoyUDir()
jv#=JoyV()
jvdir=JoyVDir()
jyaw#=JoyYaw()
jpitch#=JoyPitch()
jroll#=JoyRoll()

; Output joystick values
Text 0,0,"Move joystick to output values onto screen"
Text 0,20,"JoyU(): "+ju#
Text 0,40,"JoyUDir(): "+judir
Text 0,60,"JoyV(): "+jv#
Text 0,80,"JoyVDir(): "+jvdir
Text 0,100,"JoyYaw(): "+jyaw#
Text 0,120,"JoyPitch(): "+jpitch#
Text 0,140,"JoyRoll(): "+jroll#

Flip
Cls

Wend