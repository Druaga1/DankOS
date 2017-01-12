fat12_load_chain:

; ***********************************************************************
;     Dumps the whole cluster chain (FAT12) into the specified buffer
; ***********************************************************************

;	IN:
;		AX - cluster number
;		ES:BX - target buffer
;		DL - drive number
;	OUT: Chain loaded into a buffer

push eax
push ebx
push ecx
push edx

push ds

mov cx, KernelSpace					; Point DS to kernel space
mov ds, cx

mov word [.Cluster], ax			; Save starting cluster
mov word [.BufferOffset], bx	; Save buffer offset
mov byte [.CurrentDrive], dl	; Save current drive

; Fetch some metadata from the BPB

mov ebx, 0x0D			; Address of the Sectors per cluster constant
push 0x24
int 0x80				; Load byte from address

mov byte [.SectorsPerCluster], al

mov ebx, 0x0E			; Address of the Reserved sectors constant
push 0x25
int 0x80				; Load word from address

mov word [.StartOfFAT], ax		; Save result

mov ebx, 0x10			; Address of the Number of FATs constant
push 0x24
int 0x80				; Load word from address

mov byte [.NumberOfFATs], al	; Save result

mov ebx, 0x11			; Address of the Root entries constant
push 0x25
int 0x80				; Load word from address

mov word [.RootEntries], ax		; Save result

mov ebx, 0x16			; Address of the Sectors per FAT constant
push 0x25
int 0x80				; Load word from address

mov word [.SizeOfFAT], ax		; Save result

							; Get sectors per cluster in bytes
mov ax, word [.SectorsPerCluster]
mov bx, 512
mul bx
mov word [.SectorsPerClusterInBytes], ax

xor eax, eax							; Get start of FAT in bytes
mov ax, word [.StartOfFAT]
mov ebx, 512
mul ebx
mov dword [.StartOfFATInBytes], eax

;  Calculate the start and size of the root directory

; Start = reserved_sectors + (number_of_FATs * sectors_per_FAT)
; Size = (root_entries * 32) / bytes_per_sector

; Number of fats * sector per fat in AX

mov ax, word [.SizeOfFAT]
mov bl, byte [.NumberOfFATs]
xor bh, bh
mul bx

add ax, word [.StartOfFAT]			; Add reserved sectors

mov word [.StartOfRoot], ax				; Save result in memory

; Root entries * 32

mov ax, 32
mul word [.RootEntries]

xor dx, dx								; XOR DX for division
div word [.BytesPerSector]

mov word [.SizeOfRoot], ax			; Save result in memory

; Start of data = (Start of root - 2) + size of root

mov ax, word [.StartOfRoot]
sub ax, 2						; Subtract 2 to get LBA
add ax, word [.SizeOfRoot]
mov word [.DataStart], ax

; Load chain

mov ax, word [.Cluster]			; Prepare to enter loop
mov bx, word [.BufferOffset]

.cluster_loop:

mov dl, byte [.CurrentDrive]	; Retrieve current drive
cmp ax, 0xFF7				; Is the last cluster?
jg .done					; If yes, we finished

; Multiply ax by the sectors per cluster

mul byte [.SectorsPerCluster]

add ax, word [.DataStart]	; Add the data start offset
xor cx, cx
mov cl, byte [.SectorsPerCluster]
push 0x23					; Read!
int 0x80
add bx, word [.SectorsPerClusterInBytes]	; Move buffer up the bytes per cluster size

push bx
mov ax, word [.Cluster]		; Divide cluster by 2
mov bx, 2
xor dx, dx
div bx
add ax, word [.Cluster]		; Add this to get CLUSTER*1.5 (12 bit)
xor ebx, ebx
mov bx, ax

push dx

add ebx, dword [.StartOfFATInBytes]
mov dl, byte [.CurrentDrive]
push 0x25
int 0x80					; Fetch cluster

pop dx

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

.done:

pop ds

pop edx
pop ecx
pop ebx
pop eax

ret


.DataStart					dw	0x0000
.SizeOfFAT					dw	0x0000
.CurrentDrive				db	0x00
.Cluster					dw	0x0000
.BufferOffset				dw	0x0000
.SectorsPerCluster			db	0x00
.SectorsPerClusterInBytes	dw	0x0000
.StartOfFAT					dw	0x0000
.StartOfFATInBytes			dd	0x00000000
.NumberOfFATs				db	0x00
.StartOfRoot				dw	0x0000
.SizeOfRoot					dw	0x0000
.RootEntries				dw	0x0000
.BytesPerSector				dw	512
