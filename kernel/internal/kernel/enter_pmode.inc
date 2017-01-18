enter_pmode:

bits 16

cli						; Disable interrupts

pop word [.ReturnOffset]	; Save return offset
add esp, 2					; Discard return selector

mov dword [.EAXValue], eax		; Save EAX

lidt [IDT]				; Load the pmode IDT

mov eax, cr0			; Enable bit 0 of cr0 and enter protected mode
or eax, 00000001b
mov cr0, eax

jmp 0x08:.pmode

.pmode:					; Now in 32-bit protected mode

bits 32

mov ax, word [CPUStatus.DS]
mov ds, ax
mov ax, word [CPUStatus.ES]
mov es, ax
mov ax, word [CPUStatus.FS]
mov fs, ax
mov ax, word [CPUStatus.GS]
mov gs, ax
mov ax, word [CPUStatus.SS]
mov ss, ax

mov esp, dword [CPUStatus.ESP]

push 0x08
xor eax, eax
mov ax, word [.ReturnOffset]
push eax

mov eax, dword [.EAXValue]	; Restore EAX

retf

bits 16

.EAXValue		dd	0x00000000
.ReturnOffset	dw	0x0000
