
Context "Timer"

timer = CreateTimer(60)
Expect timer <> 0, "Created timer"

WaitTimer(timer)

FreeTimer(timer)
