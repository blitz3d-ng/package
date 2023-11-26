Context "System"

Delay 1

Expect GetEnv( "PATH" )<>"", "PATH is present"

Expect DPIScaleX()>0.0, "DPI is greater than 0"
Expect DPIScaleY()>0.0, "DPI is greater than 0"

; TODO: add a function to detect a display
ScreenWidth()
ScreenHeight()

Expect SystemProperty( "appdir" )<>"", "appdir property is present"

Expect MilliSecs()>0, "Can get clock time in ms"

Expect CurrentDate$()<>"", "Gets the current date"

Expect CurrentTime$()<>"", "Gets the current time"
