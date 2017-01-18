IDT:

dw .IDTEnd - .IDTStart - 1	; IDT size
dd .IDTStart				; IDT start

.IDTStart:

times 0x80 dq 0

; Interrupt 0x80

dw system_call				; Interrupt offset
dw 0x0008					; Kernel code selector
db 0x00						; Unused
db 11101110b				; Type
dw 0						; Offset, high 16-bits

.IDTEnd:
