
Context "Legacy"

mesg$ = "the true message"
Goto jumpAhead
mesg = "never gonna happen"
.jumpAhead
Expect mesg = "the true message", "should have the right message"

; TODO: implement
;value = 1
;Gosub label1
;Expect value = 2, "value = 2"
;Goto finish
;.label1
;value = 2
;Return
;.finish
