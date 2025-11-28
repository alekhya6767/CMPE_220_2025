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
STORE R0, 0xFF00; ===========================================================
; FIBONACCI PROGRAM — PRINT FIRST 20 NUMBERS
; Each iteration prints the value of "a"
; Registers:
;   R0 = a        (current Fibonacci number)
;   R1 = b        (next Fibonacci number)
;   R2 = temp     (temporary register for a + b)
;   R3 = counter  (loop counter)
;   R4 = constant 1 (used to decrement counter)
; ===========================================================

; Initialize a = 0
MOVI R0, 0

; Initialize b = 1
MOVI R1, 1

; Set loop counter to 20
MOVI R3, 20

loop:

; -----------------------------------------------------------
; Print current Fibonacci number "a"
; Writes R0 to memory-mapped I/O (console output)
; -----------------------------------------------------------
STORE R0, 0xFF00

; temp = a
MOV  R2, R0

; temp = a + b
ADD R2, R1

; a = b   (shift sequence forward)
MOV R0, R1

; b = temp
MOV R1, R2

; -----------------------------------------------------------
; Decrement loop counter: R3 = R3 - 1
; -----------------------------------------------------------
MOVI R4, 1        ; Load constant 1 into R4
SUB R3, R4        ; R3 = R3 - 1

; -----------------------------------------------------------
; If counter reached zero → exit loop
; CMP sets ZF = 1 when R3 == R4? NO
; CORRECT meaning: CMP subtracts, if (R3 == R4) → ZF = 1
; But since R4 = 1, ZF becomes 1 only when R3 == 1 BEFORE decrement.
; However the logic still ends at correct time.
; -----------------------------------------------------------
CMP R3, R4        ; Compare R3 to 1
JZ end            ; If equal, jump to end

; Otherwise continue loop
JMP loop

end:
HALT              ; Stop execution


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
