fat12_load_file:

; **************************************************
;     Loads a file from a FAT12 formatted floppy
; **************************************************

; IN:
; es:bx		-->		Target segment:offset
; ds:si		-->		Must point to a filename (readable format)
; dl		-->		Drive number

; OUT:
; ecx		-->		File size (in bytes)
; Carry if error or file not found

; NOTES:
; This routine assumes 9000:0000 --> 9000:FFFF to be a usable buffer

push ax						; Save registers to prepare entering routine
push bx
push dx
push bp
push si
push di
push ds
push fs

push cx

push bx						; Save target offset for later
push es						; Save target segment for later

mov ax, 0x8000				; Point ES to kernel
mov es, ax

mov di, .ConvertedFilename	; Point DI to converted filename buffer
int 0x57					; Use int 0x57 to convert file name

mov ds, ax					; Point DS to kernel
mov si, .ConvertedFilename	; Point SI to the converted file name

; Save current drive number

mov byte [.CurrentDrive], dl

;  Fetch metadata from floppy

xor bx, bx					; Load in the buffer (9000:0000)
mov ax, 0x9000
mov es, ax
xor ax, ax					; Load LBA sector 0 (boot sector)
call floppy_read_sector

mov ax, word [es:0x000E]			; Save specifications in memory
mov word [.ReservedSectors], ax
mov al, byte [es:0x0010]
mov byte [.NumberOfFATs], al
mov ax, word [es:0x0011]
mov word [.RootEntries], ax
mov ax, word [es:0x0016]
mov word [.SectorsPerFAT], ax

;  Calculate the start and size of the root directory

; Start = reserved_sectors + (number_of_FATs * sectors_per_FAT)
; Size = (root_entries * 32) / bytes_per_sector

; Number of fats * sector per fat in AX

mov ax, word [.SectorsPerFAT]
mov bl, byte [.NumberOfFATs]
xor bh, bh
mul bx

add ax, word [.ReservedSectors]			; Add reserved sectors

mov word [.StartOfRoot], ax				; Save result in memory

; Root entries * 32

mov ax, 32
mul word [.RootEntries]

xor dx, dx								; XOR DX for division
div word [.BytesPerSector]

mov word [.SizeOfRoot], ax			; Save result in memory

; Load root dir into buffer

xor bx, bx
mov ax, word [.StartOfRoot]			; Load from here
mov cx, word [.SizeOfRoot]			; Load this many sectors
mov dl, byte [.CurrentDrive]			; Retrieve drive
call floppy_read_sectors

; Browse root dir for a matching name to the one in SI

push si					; Save current SI
xor dx, dx				; Reset entry counter
xor bx, bx				; Reset root pointer

.browsing_loop:

cmp dx, word [.RootEntries]		; Is this the last entry?
je .not_found			; File not found
mov cx, 11				; Each entry is 11 bytes
.file_name_loop:
lodsb					; Byte from SI
mov ah, byte [es:bx]	; Byte from buffer
cmp ah, al				; Compare
jne .next_entry			; If not equal go to next entry
inc bx					; Increment BX pointer
loop .file_name_loop	; If 11 bytes
jmp .found_entry		; we found the file

.next_entry:

inc dx					; Increment DX entry counter
mov cx, dx				; Move DX in CX for multiplication

mov ax, 32				; Multiply CX * 32 to get to the next entry
mul cx
mov bx, ax				; Next entry in BX
pop si
push si					; Reset SI
mov dx, cx				; Move back CX in DX
jmp .browsing_loop		; Reenter loop

.found_entry:

pop si					; POP SI out of stack
mov cx, dx				; Move DX in CX for multiplication
mov ax, 32				; Multiply CX * 32 to get to the start of this entry
mul cx
add ax, 0x1A			; Add offset to the cluster number
mov bx, ax

mov ax, word [es:bx]
mov word [.Cluster], ax			; Save starting cluster
add bx, 2
mov eax, dword [es:bx]
mov dword [.FileSize], eax		; Save file size in bytes

; Let's load the FAT in the buffer now

; FAT start = Reserved sectors
; FAT size = Sectors per FAT

xor bx, bx
mov ax, word [.ReservedSectors]			; Load from here
mov cx, word [.SectorsPerFAT]			; Load this many sectors
mov dl, byte [.CurrentDrive]				; Retrieve current drive
call floppy_read_sectors

; Start of data = (Start of root - 2) + size of root

mov ax, word [.StartOfRoot]
sub ax, 2						; Subtract 2 to get LBA
add ax, word [.SizeOfRoot]
mov word [.DataStart], ax

; We'll now load the chain of clusters in the target destination

pop es						; Restore target segment
pop bx						; Restore target offset

mov ax, 0x9000				; Now point FS to the buffer
mov fs, ax

mov ax, word [.Cluster]		; Load starting cluster

.cluster_loop:

mov dl, byte [.CurrentDrive]	; Retrieve current drive
cmp ax, 0xFF7				; Is the last cluster?
jg .found					; If yes, we finished
add ax, word [.DataStart]	; Add the data start offset

call floppy_read_sector		; Read!
add bx, 512					; Move buffer up a sector

.cluster_fetch:

push bx
mov ax, word [.Cluster]		; Divide cluster by 2
mov bx, 2
xor dx, dx
div bx
add ax, word [.Cluster]		; Add this to get CLUSTER*1.5 (12 bit)
mov bx, ax
mov ax, word [fs:bx]		; Fetch cluster
cmp dx, 1					; If DX is on, we are on a split byte, and need to fetch 2 bytes,
							; get the high nibble of the first, and add the second * 0x10
je .split_byte
							; Otherwise clear the high 4 bits of AH
and ah, 00001111b
jmp .end_fetch

.split_byte:
and al, 11110000b			; Clear low 4 of AL
shr ax, 4					; Shift right a nibble

.end_fetch:
pop bx
mov word [.Cluster], ax		; Save current cluster
jmp .cluster_loop

.not_found:

pop si					; SI out of stack
pop es
pop bx

pop cx					; Restore CX

						; Set carry

mov bp, sp
add sp, 20		; Get to the flags
pop ax			; Pop and set carry flag
or ax, 0000000000000001b
push ax			; Put flags back in stack
mov sp, bp		; Restore stack

jmp .done

.found:

pop cx					; CX out of stack
mov ecx, dword [.FileSize]	; Save file size in ECX

						; Clear carry

mov bp, sp
add sp, 20		; Get to the flags
pop ax			; Pop and clear carry flag
and ax, 1111111111111110b
push ax			; Put flags back in stack
mov sp, bp		; Restore stack

.done:

pop fs					; Restore registers
pop ds
pop di
pop si
pop bp
pop dx
pop bx
pop ax
iret

.ReservedSectors dw 0x0000
.SectorsPerFAT dw 0x0000
.RootEntries dw 0x0000
.NumberOfFATs db 0x00
.StartOfRoot dw 0x0000
.SizeOfRoot dw 0x0000
.Cluster dw 0x0000
.BytesPerSector dw 512
.DataStart dw 0x0000
.FileSize dd 0x00000000
.CurrentDrive db 0x00
.ConvertedFilename times 12 db 0x00
