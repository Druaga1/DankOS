exit_pmode:

bits 32

; Save pre-real mode CPU status

pop dword [.ReturnOffset]	; Save return offset
add esp, 2					; Discard return selector

mov dword [.EAXValue], eax	; Save original EAX
mov dword [CPUStatus.ESP], esp	; Save ESP

mov ax, ds
mov word [CPUStatus.DS], ax
mov ax, es
mov word [CPUStatus.ES], ax
mov ax, fs
mov word [CPUStatus.FS], ax
mov ax, gs
mov word [CPUStatus.GS], ax
mov ax, ss
mov word [CPUStatus.SS], ax

lidt [.RealModeIVT]			; Load the real mode IVT (base 0, limit 0x3FF)

jmp 0x28:.data_segments

bits 16

.data_segments:

mov ax, 0x30
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

mov eax, cr0
and eax, 11111110b
mov cr0, eax

jmp KernelSpace:.real_mode

.real_mode:

mov ax, KernelSpace
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

mov esp, CPUStatus.ReservedStack

mov eax, dword [.ReturnOffset]
push KernelSpace
push ax

sti

mov eax, dword [.EAXValue]		; Restore EAX

retf

.EAXValue			dd	0x00000000
.ReturnOffset		dd	0x00000000

.RealModeIVT:

dw 0x3FF
dd 0x00000000
