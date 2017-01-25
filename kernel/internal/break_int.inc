break_int:

push si
push ds

mov si, KernelSpace
mov ds, si

mov si, .msg
push 0x02
int 0x80

mov byte [BreakFlag], 0x01

pop ds
pop si

iret

.msg db 0x0A, 'Kernel: Aborting execution via CTRL+Break.', 0x0A, 0x00
