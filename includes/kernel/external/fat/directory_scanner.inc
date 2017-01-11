directory_scanner:

; **************************************************
;     Scans the content of the current directory
; **************************************************

; IN:
; ax		-->		Directory entry number

; OUT:
; ax		-->		Raw time
; bx		-->		Raw date
; es:di		-->		File name
; ecx		-->		Size
; dh		-->		Directory flag (0x00 if file, 0xFF if directory)
; dl		-->		0xFF if entry not found, 0x00 if found

push si
push di
push ds
push es

mov bx, KernelSpace
mov ds, bx

mov word [.EntryNumber], ax
mov word [.TargetBuffer], di
mov ax, es
mov word [.TargetSegment], ax
mov word [.EntryCounter], 0x0000

mov es, bx

mov si, CurrentDirectoryCache

.next_entry:

mov di, .FatNameBuffer
lodsb	; Byte from the directory table, first of entry
dec si
test al, al		; End of table?
jz .failure

mov cx, 11
rep movsb
xor al, al
stosb

; Check for special reserved entries

mov al, byte [si-11]	; Check for deleted entry
cmp al, 0xE5
je .skip_entry

mov al, byte [si]		; Check for a vfat entry (ignore it)
cmp al, 0x0F
je .skip_entry

mov al, byte [si]		; Check for a dir entry
and al, 0x10
jz .no_directory
mov byte [.DirectoryFlag], 0xFF
jmp .directory_check_done
.no_directory:
mov byte [.DirectoryFlag], 0x00
.directory_check_done:

mov ax, word [si+11]	; Get time
mov word [.RawTime], ax

mov ax, word [si+13]	; Get date
mov word [.RawDate], ax

mov eax, dword [si+17]	; Get size
mov dword [.FileSize], eax

mov ax, word [.EntryNumber]
cmp word [.EntryCounter], ax
je .success

inc word [.EntryCounter]

.skip_entry:

add si, 21		; Get to the next entry
jmp .next_entry


.failure:

xor dl, dl
not dl
jmp .done

.success:

mov si, .FatNameBuffer				; Convert name
mov di, word [.TargetBuffer]
mov ax, word [.TargetSegment]
mov es, ax

call fat_name_to_string

xor dl, dl

.done:

mov dh, byte [.DirectoryFlag]
mov ax, word [.RawTime]
mov bx, word [.RawDate]
mov ecx, dword [.FileSize]

pop es
pop ds
pop di
pop si
iret



.DirectoryFlag		db	0x00
.EntryNumber		dw	0x0000
.EntryCounter		dw	0x0000
.TargetBuffer		dw	0x0000
.TargetSegment		dw	0x0000
.RawDate			dw	0x0000
.RawTime			dw	0x0000
.FileSize			dd	0x00000000
.FatNameBuffer		times 12 db 0x00
