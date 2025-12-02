; ========================================
; Recursive factorial using CALL/RET
; n = 5
; R0 on exit = 5! = 120
; ========================================

        MOVI R0, 5          ; R0 = n
        CALL fact           ; R0 = fact(5)
        STORE R0, 0xFF00    ; print 120
        HALT

; ========================================
; fact(n):
; if (n == 0) return 1
; else        return n * fact(n - 1)
; ========================================

fact:
        ; ---- base case: if R0 == 0 → return 1 ----
        MOVI R1, 0          ; R1 = 0
        CMP  R0, R1         ; compare n with 0
        JZ   base_case      ; if n == 0 → base_case

        ; ---- recursive case ----
        ; Save n on stack
        PUSH R0             ; [SP] = n, SP -= 2

        ; Call fact(n - 1)
        MOVI R2, 1          ; R2 = 1
        SUB  R0, R2         ; R0 = n - 1
        CALL fact           ; after return: R0 = fact(n - 1)

        ; Restore n from stack
        POP  R4             ; R4 = original n, SP += 2

        ; ---- multiply: R0 = fact(n-1) * n ----
        MOV  R5, R0         ; R5 = fact(n - 1)
        MOVI R0, 0          ; R0 = accumulator = 0
        MOVI R3, 0          ; R3 = counter = 0
        MOVI R2, 1          ; R2 = 1 (step)

mul_loop:
        CMP  R3, R4         ; while (counter < n)
        JZ   mul_done       ; if counter == n → done
        ADD  R0, R5         ; acc += fact(n - 1)
        ADD  R3, R2         ; counter++
        JMP  mul_loop

mul_done:
        RET                 ; return to caller, R0 has n!

base_case:
        MOVI R0, 1          ; fact(0) = 1
        RET
