sleep:

; IN: ECX: Number of ticks of inactivity

push ax
push ds

mov ax, KernelSpace
mov ds, ax

mov dword [ClockTicks], 0		; Reset tick counter

.loop:

cmp dword [ClockTicks], ecx
je .done
jmp .loop

.done:

pop ds
pop ax
iret
