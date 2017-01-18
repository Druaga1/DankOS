exit_pmode:

bits 32

push eax

lidt [.RealModeIVT]			; Load the real mode IVT (base 0, limit 0x3FF)

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



.RealModeIVT:

dw 0x3FF
dd 0x00000000
