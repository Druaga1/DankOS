fat_write_entry:

; ************************************************************
;     Writes a new entry into the current directory buffer
; ************************************************************

; IN: DS:SI file name
;     DI = starting cluster
;     AL = attributes
;     BX = raw FAT time
;     ECX = file size
;     DX = raw FAT date

; OUT: Nothing

push eax
push ebx
push ecx
push edx
push si
push di
push ds
push es

mov ax, KernelSpace
mov es, ax

mov word [es:.Cluster], di

mov di, .ConvertedName			; Convert to fat name
call string_to_fat_name

mov ds, ax

mov byte [.Attributes], al
mov word [.RawTime], bx
mov word [.RawDate], dx
mov dword [.FileSize], ecx

mov di, CurrentDirectoryCache
mov word [.EntryCounter], 0x0000

.find_empty_slot:

inc word [.EntryCounter]

mov ah, byte [es:di]	; Byte from the directory table, first of entry
cmp ah, 0xE5	; Empty entry?
jz .write_entry

mov ah, byte [es:di]	; Byte from the directory table, first of entry
test ah, ah		; End of table?
jz .write_entry

; Skip entry

mov ax, 32
mov di, CurrentDirectoryCache
mul word [.EntryCounter]
add di, ax
jmp .find_empty_slot

.write_entry:

mov si, .ConvertedName

mov cx, 11				; Copy file name
rep movsb

mov al, byte [.Attributes]		; Store attributes
stosb

xor ax, ax				; Erase unused entries
mov cx, 5
rep stosw

mov ax, word [.RawTime]		; Get time
stosw

mov ax, word [.RawDate]		; Get date
stosw

mov ax, word [.Cluster]		; Get cluster
stosw

mov eax, dword [.FileSize]	; Get size
stosd

pop es
pop ds
pop di
pop si
pop edx
pop ecx
pop ebx
pop eax
ret


.EntryCounter	dw	0x0000
.Cluster		dw	0x0000
.FileSize		dd	0x00000000
.RawDate		dw	0x0000
.RawTime		dw	0x0000
.Attributes		db	0x00
.ConvertedName	times 12 db 0x00
