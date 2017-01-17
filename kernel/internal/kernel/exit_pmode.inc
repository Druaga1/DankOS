exit_pmode:

bits 32

push eax

jmp 0x28:.data_segments

bits 16

.data_segments:

mov ax, 0x30
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

mov eax, cr0
and eax, 11111110b
mov cr0, eax

jmp KernelSpace:.real_mode

.real_mode:

mov ax, KernelSpace
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

pop eax

sti

ret
