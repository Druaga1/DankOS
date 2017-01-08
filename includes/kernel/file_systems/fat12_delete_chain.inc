fat12_delete_chain:

; ***********************************************
;     Deletes the whole cluster chain (FAT12)
; ***********************************************

;	IN:
;		AX - cluster number
;		DL - drive number
;	OUT: Chain will be deleted

push eax
push ebx
push ecx
push edx

push ds

mov cx, KernelSpace					; Point DS to kernel space
mov ds, cx

mov word [.Cluster], ax			; Save starting cluster
mov byte [.CurrentDrive], dl	; Save current drive

; Fetch some metadata from the BPB

mov ebx, 0x0E			; Address of the Reserved sectors constant
push 0x25
int 0x80				; Load word from address

mov word [.StartOfFAT], ax		; Save result

xor eax, eax							; Get start of FAT in bytes
mov ax, word [.StartOfFAT]
mov ebx, 512
mul ebx
mov dword [.StartOfFATInBytes], eax



; Delete cluster

.delete_cluster:

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
							; Otherwise keep the high 4 bits of AH, clear the rest
push bx
mov bx, ax					; Save next cluster
and bh, 00001111b
mov word [.Cluster], bx
pop bx
and ax, 1111000000000000b
push 0x32					; Write cluster to the FAT
int 0x80
jmp .end_fetch

.split_byte:
push bx
mov bx, ax
and bl, 11110000b			; Save next cluster
shr bx, 4
mov word [.Cluster], bx
pop bx
and ax, 0000000000001111b	; Clear everything but low 4 of AL
push 0x32					; Write cluster to the FAT
int 0x80

.end_fetch:

cmp word [.Cluster], 0xFF7
jg .done
jmp .delete_cluster

.done:

pop ds

pop edx
pop ecx
pop ebx
pop eax

ret


.CurrentDrive				db	0x00
.Cluster					dw	0x0000
.StartOfFAT					dw	0x0000
.StartOfFATInBytes			dd	0x00000000
