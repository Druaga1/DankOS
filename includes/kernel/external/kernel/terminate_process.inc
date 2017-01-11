terminate_process:

; ********************************************
;     Handles the termination of a process
; ********************************************

; IN: EAX --> exit code of the process

cli							; Disable interrupts

mov bx, KernelSpace				; Point GS to kernel
mov gs, bx

mov bx, word [gs:StackSegment]	; Restore last stack
mov ss, bx
mov esp, dword [gs:StackPointer]

sti							; Restore interrupts

pop dword [gs:StackPointer]	; Restore older stack in RAM
pop word [gs:StackSegment]

pop word [gs:TopSegment]	; Restore last top segment

pop gs					; Restore segment registers
pop fs
pop es
pop ds

pop ebp					; Restore GPRs
pop edi
pop esi
pop edx
pop ecx
pop ebx

iret					; Return to calling program
