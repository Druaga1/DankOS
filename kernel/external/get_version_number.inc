; *****************************************
;     Returns the version of the kernel
; *****************************************

; OUT: ES:EDI --> version number string in ASCII

get_version_number:

push esi
push ds

mov si, KernelSpace
mov ds, si

mov esi, Version

push 0x27
int 0x80

pop ds
pop esi
iret
