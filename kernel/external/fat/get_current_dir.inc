get_current_dir:

; ******************************************
;     Returns the current path in ES:EDI
; ******************************************

push esi
push ds

mov si, KernelSpace
mov ds, si

mov esi, CurrentDirectoryPath
push 0x27
int 0x80

pop ds
pop esi
iret
