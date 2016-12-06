; shl, shr, sar examples

value = 100

; multiple x 2
Print "Shift 1 bit left; Value = " + value Shl 1
; multiple x 4
Print "Shift 2 bits left; Value = " + value Shl 2
; multiple x 16
Print "Shift 4 bits left; Value = " + value Shl 4
; divide by 2
Print "Shift 1 bit right; Value = " + value Shr 1
; divide by 4
Print "Shift 2 bits right; Value = " + value Shr 2
; divide by 16
Print "Shift 4 bits right; Value = " + value Shr 4

Print "Shift by SAR 4 times = " + value Sar 4

WaitKey()
