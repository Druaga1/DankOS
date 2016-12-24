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
add sp, 20		; Get to the flags
pop dx			; Pop and set carry flag
or dx, 0000000000000001b
push dx			; Put flags back in stack
mov sp, bp		; Restore stack
jmp .done

.success:

mov bp, sp
add sp, 20		; Get to the flags
pop dx			; Pop and clear carry flag
and dx, 1111111111111110b
push dx			; Put flags back in stack
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





directory_scanner:

; **********************************************************
;     Scans the content of the directory (current drive)
; **********************************************************

; IN:
; ax		-->		Directory entry number

; OUT:
; es:di		-->		File name

; Carry if entry not found

push ax
push bx
push cx
push dx
push si
push di
push bp
push ds

mov bx, 0x9000
mov ds, bx

mov word [.EntryNumber], ax
mov word [.TargetBuffer], di
mov word [.EntryCounter], 0x0000

mov si, CurrentDirectoryCache

.next_entry:

mov di, word [.TargetBuffer]
lodsb	; Byte from the directory table, first of entry
dec si
test al, al		; End of table?
jz .failure

mov cx, 11
rep movsb
xor al, al
stosb

; Check for special reserved entries

mov al, byte [si]		; Check for a vfat entry (ignore it)
cmp al, 0x0F
je .skip_entry


mov ax, word [.EntryNumber]
cmp word [.EntryCounter], ax
je .success

inc word [.EntryCounter]

.skip_entry:

add si, 21		; Get to the next entry
jmp .next_entry


.failure:

mov bp, sp
add sp, 20		; Get to the flags
pop dx			; Pop and set carry flag
or dx, 0000000000000001b
push dx			; Put flags back in stack
mov sp, bp		; Restore stack
jmp .done

.success:

mov bp, sp
add sp, 20		; Get to the flags
pop dx			; Pop and clear carry flag
and dx, 1111111111111110b
push dx			; Put flags back in stack
mov sp, bp		; Restore stack


.done:

pop ds
pop bp
pop di
pop si
pop dx
pop cx
pop bx
pop ax
iret



.EntryNumber		dw	0x0000
.EntryCounter		dw	0x0000
.TargetBuffer		dw	0x0000



file_exists:


iret


