; ===========================================================
; FIBONACCI PROGRAM — PRINT FIRST 20 NUMBERS
; Output goes to numeric I/O port 0xFF00
; Registers:
;   R0 = a        (current Fibonacci number)
;   R1 = b        (next Fibonacci number)
;   R2 = temp     (a + b)
;   R3 = counter  (remaining count)
;   R4 = zero     (0)
;   R5 = one      (1)
; ===========================================================

        MOVI R0, 0        ; a = 0
        MOVI R1, 1        ; b = 1
        MOVI R3, 20       ; want 20 Fibonacci numbers
        MOVI R4, 0        ; R4 = 0 (for compare)
        MOVI R5, 1        ; R5 = 1 (for decrement)

loop:
        ; print current Fibonacci number a
        STORE R0, 0xFF00

        ; R2 = a
        MOV   R2, R0

        ; R2 = a + b
        ADD   R2, R1

        ; a = b
        MOV   R0, R1

        ; b = R2
        MOV   R1, R2

        ; R3 = R3 - 1
        SUB   R3, R5

        ; if R3 != 0, continue
        CMP   R3, R4
        JNZ   loop

        HALT
