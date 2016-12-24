; **********************************************************************************************
;     This file contains FAT routines that are callable from outside programs using int 0x80
; **********************************************************************************************



load_file:

; *********************************************
;     Loads a file from the specified drive
; *********************************************

; IN:
; es:bx		-->		Target segment:offset
; ds:si		-->		Must point to a filename (readable format)
; dl		-->		Drive number

; OUT:
; ecx		-->		File size (in bytes)

; Carry if failure

push ax
push bx
push dx
push si
push di
push bp
push ds
push es

mov cx, es

mov ax, 0x9000
mov es, ax

mov byte [es:.DriveNumber], dl
mov word [es:.TargetSegment], cx
mov word [es:.TargetBuffer], bx

mov di, .ConvertedName			; Convert to FAT name, and save into buffer

push 0x17
int 0x80

mov ds, ax
mov si, .ConvertedName

mov dl, byte [.DriveNumber]
call fat_load_root				; Load the root directory

call fat_get_cluster			; Get starting cluster and size

test ax, ax
jz .failure						; Check for success

mov cx, word [.TargetSegment]
mov es, cx

mov bx, word [.TargetBuffer]
mov dl, byte [.DriveNumber]
call fat12_load_chain			; Load chain into the buffer
jmp .success

.failure:

mov bp, sp
add sp, 18		; Get to the flags
pop ax			; Pop and set carry flag
or ax, 0000000000000001b
push ax			; Put flags back in stack
mov sp, bp		; Restore stack
jmp .done

.success:

mov bp, sp
add sp, 18		; Get to the flags
pop ax			; Pop and clear carry flag
and ax, 1111111111111110b
push ax			; Put flags back in stack
mov sp, bp		; Restore stack

.done:
pop es
pop ds
pop bp
pop di
pop si
pop dx
pop bx
pop ax
iret


.DriveNumber   db 0x00
.TargetSegment dw 0x0000
.TargetBuffer  dw 0x0000
.ConvertedName times 12 db 0x00

















file_exists:


iret


