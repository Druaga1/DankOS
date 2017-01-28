; *****************************************
;     Returns the version of the kernel
; *****************************************

; OUT: EAX --> version number 0x00/00/00/00

get_version_number:

push ds

mov ax, KernelSpace
mov ds, ax

mov eax, dword [VersionNumber]

pop ds
iret
