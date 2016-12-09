get_current_drive:

; *******************************************
;     Get default drive set in the kernel
; *******************************************

; IN: Nothing

; OUT: DL --> current drive

push ax
push ds

mov ax, 0x9000
mov ds, ax

mov dl, byte [CurrentDrive]

pop ds
pop ax
iret


start_new_program:

; ******************************************
;     Handles the start of a new process
; ******************************************

; IN: DS:SI --> Name of new program
;     DS:DI --> Command line switches
;     DL --> Drive number

; OUT: EAX = 0xFFFFFFFF if error or not found

push ebx					; Save general purpose registers
push ecx					; (doesn't save AX because it's the exit code register)
push edx
push esi
push edi
push ebp

push ds						; Save segment registers
push es
push fs
push gs

mov ax, 0x9000				; Point GS to kernel
mov gs, ax

push word [gs:TopSegment]	; Save last top of memory in stack
push word [gs:StackSegment]	; Save last stack segment in stack
push dword [gs:StackPointer]	; Save last stack pointer in stack

mov ax, ss
mov word [gs:StackSegment], ax	; Save new stack segment in RAM
mov dword [gs:StackPointer], esp	; Save new stack pointer in RAM

mov ax, word [gs:TopSegment]		; Prepare to load program
mov es, ax
mov bx, 0x0100					; Load at offset 0x0100
push 0x12
int 0x80						; Load program
jc .drop_out					; If file not found, drop out
cmp ecx, 0x0000FFFF				; Test if program is too big (> 64KB)
jg .drop_out					; If it is, drop out

add cx, 0x0100					; Add the kernel reserved space to the file size
shr cx, 4						; Shift CX right by 4 bits to get size in 16 bytes blocks
inc cx							; Adjust CX
add word [gs:TopSegment], cx		; Reduce available memory

mov si, di						; Copy command line switches location in DS:DI
mov es, ax						; Prepare ES:DI to copy the switches
xor di, di

push ax							; Save AX for later

.copy_switches_loop:
lodsb							; Byte from DS:SI
stosb							; Copy to ES:DI
test al, al						; Test for terminator
jz .copy_switches_done
jmp .copy_switches_loop
.copy_switches_done:

pop ax							; Restore AX

cli								; Disable interrupts
mov cx, word [gs:TopSegment]		; Prepare the stack segment
mov ss, cx
mov esp, 0x1f0						; Prepare the stack pointer
sti								; Restore interrupts

add word [gs:TopSegment], 0x20		; Allocate 512 more bytes for the stack

mov ds, ax						; Prepare all segment registers (AX was already correctly loaded)
mov fs, ax
mov gs, ax

push ax							; Prepare stack for retf
push 0x0100

xor eax, eax					; Flush all GPRs
xor ebx, ebx
xor ecx, ecx
xor edx, edx
xor esi, esi
xor edi, edi
xor ebp, ebp

retf							; Launch program :D

.drop_out:

mov eax, 0xFFFFFFFF		; Set EAX to 0xFFFFFFFF
push 0x00
int 0x80				; Terminate process instantly


terminate_process:

; ********************************************
;     Handles the termination of a process
; ********************************************

; IN: EAX --> exit code of the process

cli							; Disable interrupts

mov bx, 0x9000				; Point GS to kernel
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


allocate_memory:

; **********************************************
;     Handles a program's request for memory
; **********************************************

; IN: AX --> Amount of memory to allocate

; OUT: CX --> Allocated segment (for data segment registers)

push ax
push gs

mov cx, 0x9000				; Point GS to kernel
mov gs, cx

mov cx, word [gs:TopSegment]	; Retrieve current top of used memory
shr ax, 4					; Get memory size in 16 byte blocks
inc ax						; Adjust AX
add word [gs:TopSegment], ax	; Allocate memory

pop gs
pop ax
iret
