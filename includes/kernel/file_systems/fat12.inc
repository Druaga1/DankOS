; Load chain FAT12		(internal)

;	IN:
;		AX - cluster number
;		ES:BX - target buffer
;		DL - drive number
;	OUT: Chain loaded into a buffer

fat12_load_chain:

push eax
push ebx
push ecx
push edx

mov word [.Cluster], ax			; Save starting cluster
mov word [.BufferOffset], bx	; Save buffer offset
mov byte [.CurrentDrive], dl	; Save currnet drive

; Fetch some metadata from the BPB

mov ebx, 0x0D			; Address of the Sectors per cluster constant
push 0x24
int 0x80				; Load byte from address

mov byte [.SectorsPerCluster], al

mov ebx, 0x0E			; Address of the Reserved sectors constant
push 0x25
int 0x80				; Load word from address

mov word [.StartOfRoot], ax		; Save result

mov ebx, 0x16			; Address of the Sectors per FAT constant
push 0x25
int 0x80				; Load word from address

mov word [.SizeOfRoot], ax		; Save result

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
add bx, cx					; Move buffer up the sectors per cluster size

push bx
mov ax, word [.Cluster]		; Divide cluster by 2
mov bx, 2
xor dx, dx
div bx
add ax, word [.Cluster]		; Add this to get CLUSTER*1.5 (12 bit)
xor ebx, ebx
mov bx, ax
push dx
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

pop edx
pop ecx
pop ebx
pop eax
ret


.DataStart				dw	0x0000
.SizeOfRoot				dw	0x0000
.CurrentDrive			db	0x00
.Cluster				dw	0x0000
.BufferOffset			dw	0x0000
.SectorsPerCluster		db	0x00
.StartOfRoot			dw	0x0000
