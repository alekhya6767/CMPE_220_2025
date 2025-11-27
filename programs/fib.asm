; ============================================
; FIXED FIBONACCI PROGRAM (20 numbers only)
; ============================================

; a = 0
MOVI R0, 0

; b = 1
MOVI R1, 1

; R3 = 20 (counter)
MOVI R3, 20

loop:

; print a
STORE R0, 0xFF00

; R2 = a
MOV  R2, R0

; R2 = a + b
ADD R2, R1

; a = b
MOV R0, R1

; b = R2
MOV R1, R2

; decrement counter by 1
MOVI R4, 1   ; load immediate 1 into R4
SUB R3, R4

; if R3 == 0 → exit
CMP R3, R4   ; compare R3 with 0 by checking subtraction result
JZ end

JMP loop

end:
HALT
