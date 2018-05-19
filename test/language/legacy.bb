
Context "Legacy"

mesg$ = "the true message"

Goto jumpAhead

mesg = "never gonna happen"

.jumpAhead

Expect mesg = "the true message", "should have the right message"
