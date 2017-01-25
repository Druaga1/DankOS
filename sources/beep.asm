org 0x100
bits 16

mov esi, music
push 0x1F
int 0x80

push 0x00				; Return
int 0x80




music:

; idk some music


db 20			; Duration
dw 440			; Note

db 5
dw 880

db 5
dw 0			; This is a pause

db 10
dw 440

dw 0			; Song end marker
