timer_int:

push ax
push ds

mov ax, KernelSpace
mov ds, ax

inc dword [ClockTicks]

pop ds
pop ax

iret
