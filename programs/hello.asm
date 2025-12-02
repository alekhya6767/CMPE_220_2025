; =============================================================
; HELLO WORLD — prints characters to 0xFF10
; Each MOVI loads an ASCII code into R0, STORE writes a char
; =============================================================

        ; 'H'
        MOVI R0, 72
        STORE R0, 0xFF10

        ; 'e'
        MOVI R0, 101
        STORE R0, 0xFF10

        ; 'l'
        MOVI R0, 108
        STORE R0, 0xFF10

        ; 'l'
        MOVI R0, 108
        STORE R0, 0xFF10

        ; 'o'
        MOVI R0, 111
        STORE R0, 0xFF10

        ; ' '
        MOVI R0, 32
        STORE R0, 0xFF10

        ; 'W'
        MOVI R0, 87
        STORE R0, 0xFF10

        ; 'o'
        MOVI R0, 111
        STORE R0, 0xFF10

        ; 'r'
        MOVI R0, 114
        STORE R0, 0xFF10

        ; 'l'
        MOVI R0, 108
        STORE R0, 0xFF10

        ; 'd'
        MOVI R0, 100
        STORE R0, 0xFF10

        ; newline (optional)
        MOVI R0, 10
        STORE R0, 0xFF10

        HALT
