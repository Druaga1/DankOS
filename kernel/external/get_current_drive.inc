get_current_drive:

; *******************************************
;     Get default drive set in the kernel
; *******************************************

; IN: Nothing

; OUT: DL --> current drive

push ax
push ds

mov ax, KernelSpace
mov ds, ax

mov dl, byte [CurrentDrive]

pop ds
pop ax
iret
