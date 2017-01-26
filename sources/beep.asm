org 0x100
bits 16

%include 'includes/music.inc'

mov esi, music
push 0x1F
int 0x80

mov esi, msg
push 0x02
int 0x80

push 0x03
int 0x80

push 0x18
int 0x80

push 0x2A
int 0x80

push 0x00				; Return
int 0x80


msg db "press a key to stop playing", 0x00

music:

; idk some music


db 5
dw C_oct_below

db 5
dw D_oct_below

db 5
dw E_oct_below

db 5
dw F_oct_below

db 5
dw G_oct_below

db 5
dw A

db 5
dw B

db 5
dw C

db 5
dw B

db 5
dw A

db 5
dw G_oct_below

db 5
dw F_oct_below

db 5
dw E_oct_below

db 5
dw D_oct_below

db 20
dw C_oct_below

db 0
