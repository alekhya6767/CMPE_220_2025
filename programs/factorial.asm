; ============================================================
; RECURSIVE FACTORIAL ON SOFTWARE CPU
; ------------------------------------------------------------
; Computes n! for n <= 7 using:
;   - CALL / RET
;   - PUSH / POP (stack for saving n and return addresses)
;   - Simple multiply loop using repeated addition
;
; Register conventions in this program:
;   R0 : input n, and final result factorial(n)
;   R1 : temporary / copy of n or partial results
;   R2 : temporary (loop counter)
;   R3 : constant 1 (used for decrement/increment)
;   R4 : saved 'n' value on each recursion (after POP)
;   R5 : spare (not strictly required here)
;
; Stack:
;   - CALL pushes return PC, RET pops it.
;   - PUSH R0 saves the current n before the recursive call.
; ============================================================

        ; Choose input n
        MOVI R0, 5          ; R0 = 5  (change this value to test other n)

        ; Call recursive function fact(R0)
        CALL fact           ; result will be returned in R0

        ; Print result via memory-mapped I/O at 0xFF00
        STORE R0, 0xFF00    ; prints factorial(n) as a decimal number

        HALT                ; stop emulator


; ============================================================
; FUNCTION: fact
; ------------------------------------------------------------
; Contract:
;   Input : R0 = n
;   Output: R0 = n!
;
; Logic:
;   if (n == 0) return 1
;   else        return n * fact(n - 1)
; ============================================================

fact:
        ; ----- Base case: if n == 0 → return 1 -----
        MOVI R1, 0          ; R1 = 0
        CMP  R0, R1         ; compare n (R0) with 0
        JZ   fact_base      ; if n == 0, jump to base_case

        ; ----- Recursive case: n * fact(n - 1) -----

        ; Save current n on the stack
        PUSH R0             ; push n

        ; Compute n - 1 and call fact(n - 1)
        MOVI R3, 1          ; R3 = 1  (constant for decrement/increment)
        SUB  R0, R3         ; R0 = n - 1
        CALL fact           ; recursive call, returns fact(n - 1) in R0

        ; Restore original n from stack
        POP  R4             ; R4 = original n

        ; ----- Multiply: R0 = fact(n-1) * n -----
        MOV  R1, R0         ; R1 = fact(n - 1)
        MOVI R0, 0          ; R0 = accumulator = 0
        MOVI R2, 0          ; R2 = loop counter = 0

fact_mul_loop:
        ; loop while R2 < R4
        CMP  R2, R4         ; compare counter with n
        JZ   fact_mul_done  ; if R2 == n, multiplication done

        ADD  R0, R1         ; R0 += fact(n - 1)
        ADD  R2, R3         ; R2 += 1
        JMP  fact_mul_loop  ; repeat

fact_mul_done:
        RET                 ; return to caller, R0 holds n!

; ----- Base case implementation -----
fact_base:
        MOVI R0, 1          ; 0! = 1
        RET                 ; return to caller
