get_current_dir:

; *****************************************
;     Returns the current path in ES:DI
; *****************************************

push ax
push si
push ds

mov ax, KernelSpace
mov ds, ax

mov si, CurrentDirectoryPath
push 0x27
int 0x80

pop ds
pop si
pop ax
iret
