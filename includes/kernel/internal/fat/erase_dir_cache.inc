erase_dir_cache:

push ax
push es
push di

mov ax, KernelSpace
mov es, ax

mov di, CurrentDirectoryCache
mov cx, 0x2000
xor ax, ax

rep stosw

pop di
pop es
pop ax
ret
