; **********************************************
;     Renders the current disk cache invalid
; **********************************************

invalid_cache:

push ax
push ds

mov ax, KernelSpace
mov ds, ax

mov byte [CacheStatus], 0x00

pop ds
pop ax
iret
