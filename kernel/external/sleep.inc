sleep:

; IN: ECX: Number of ticks of inactivity

push ax
push ecx
push ds

mov ax, KernelSpace
mov ds, ax

add ecx, dword [ClockTicks]		; Find target tick

.loop:

cmp dword [ClockTicks], ecx
je .done
jmp .loop

.done:

pop ds
pop ecx
pop ax
iret
