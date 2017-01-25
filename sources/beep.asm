org 0x100
bits 16

mov ecx, 440			; Play A 440hz
push 0x22
int 0x80

mov ecx, 50				; Sleep 50 ticks
push 0x1D
int 0x80

push 0x1E				; Stop speaker
int 0x80

push 0x00				; Return
int 0x80
