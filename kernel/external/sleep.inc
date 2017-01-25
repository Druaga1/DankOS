sleep:

; IN: ECX: Number of ticks of inactivity

test ecx, ecx
jz .quick_exit

sti			; Enable back interrupts

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

cli

.quick_exit:
iret
