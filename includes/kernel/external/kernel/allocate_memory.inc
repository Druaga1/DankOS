allocate_memory:

; **********************************************
;     Handles a program's request for memory
; **********************************************

; IN: AX --> Amount of memory to allocate

; OUT: CX --> Allocated segment (for data segment registers)

push ax
push gs

mov cx, KernelSpace				; Point GS to kernel
mov gs, cx

mov cx, word [gs:TopSegment]	; Retrieve current top of used memory
shr ax, 4					; Get memory size in 16 byte blocks
inc ax						; Adjust AX
add word [gs:TopSegment], ax	; Allocate memory

pop gs
pop ax
iret
