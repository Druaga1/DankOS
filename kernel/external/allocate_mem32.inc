allocate_mem32:

; *****************************************************************
;     Handles a program's request for memory in extended memory
; *****************************************************************

; IN: EAX --> Amount of memory to allocate

; OUT: ECX --> Beginning of allocated memory (flat address relative to program segment)

push eax
push gs

mov cx, KernelSpace				; Point GS to kernel
mov gs, cx

mov ecx, dword [gs:TopMemory]	; Retrieve current top of used memory
add dword [gs:TopMemory], eax	; Allocate memory

xor eax, eax
mov ax, ds
shl eax, 4

sub ecx, eax

pop gs
pop eax
iret
