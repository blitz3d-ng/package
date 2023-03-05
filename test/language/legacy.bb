
Context "Legacy"

; ;;
; ; Goto
; mesg$ = "the true message"
; Goto jumpAhead
; mesg = "never gonna happen"
; .jumpAhead
; Expect mesg = "the true message", "should have the right message"

;;
; Gosub
value = 1

Gosub label1
Goto finish

.label1
value = 2
Return

If value=1 Then ; here to cover a particular codegen error
	Gosub label1
	Goto finish
End If

value = 3 ; should never happen

.finish

Expect value = 2, "value = 2"
