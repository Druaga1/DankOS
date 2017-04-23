check_bin_extension:

push ax
push dx
push esi
push edi

push es

mov ax, ds      ; Convert file name to uppercase
mov es, ax
mov edi, esi
push 0x15
int 0x80

mov ax, KernelSpace
mov es, ax

.loop:
a32 o32 lodsb
cmp al, '.'
je .check_extension
test al, al
jz .no_bin
jmp .loop

.check_extension:

mov edi, .bin_string
push 0x08
int 0x80

test dl, dl
jz .no_bin

.bin:
clc
jmp .done

.no_bin:
stc

.done:

pop es

pop edi
pop esi
pop dx
pop ax

ret

.bin_string db 'BIN', 0x00
