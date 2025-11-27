; =============================================================
; HELLO WORLD — prints numeric ASCII codes
; =============================================================

MOVI R0, 72
STORE R0, 0xFF00

MOVI R0, 101
STORE R0, 0xFF00

MOVI R0, 108
STORE R0, 0xFF00

MOVI R0, 108
STORE R0, 0xFF00

MOVI R0, 111
STORE R0, 0xFF00

; space
MOVI R0, 32
STORE R0, 0xFF00

; W
MOVI R0, 87
STORE R0, 0xFF00

; o
MOVI R0, 111
STORE R0, 0xFF00

; r
MOVI R0, 114
STORE R0, 0xFF00

; l
MOVI R0, 108
STORE R0, 0xFF00

; d
MOVI R0, 100
STORE R0, 0xFF00

HALT
