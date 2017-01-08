floppy_read_byte:

; ***********************************************************
;     Reads a byte from a specific location from a floppy
; ***********************************************************

; IN:
; EBX = Byte address
; DL = Drive number

; OUT:
; AL = Byte read

push ebx
push ecx
push edx
push ds
push es

mov cx, KernelSpace			; Prepare DS and ES
mov ds, cx
mov es, cx

push eax				; Save EAX to save the other parts of the register

push dx					; Save target drive for later

; Calculate the sector where the byte is found

xor edx, edx			; Prepare EDX
mov eax, ebx			; Put the address in EAX
mov ebx, 512			; Prepare to divide by 512
div ebx					; Divide!

mov word [.target_sector], ax
mov word [.target_offset], dx

pop dx

; Check if the cache if useful

cmp byte [CacheStatus], 0xFF
jne .cache_miss
cmp ax, word [SectorInCache]
jne .cache_miss
cmp dl, byte [DriveInCache]
jne .cache_miss

.cache_hit:

; The sector is already loaded in the cache, read the byte and return

mov bx, word [.target_offset]
pop eax							; Restore the rest of EAX
add bx, DiskCache
mov al, byte [bx]
jmp .done

.cache_miss:

; The cache is not useful, we need to fetch the sector from the drive

mov byte [CacheStatus], 0xFF				; Flag cache as enabled
mov ax, word [.target_sector]
mov bx, DiskCache							; Target buffer is cache
mov cx, 1									; Read 1 sector
push 0x23
int 0x80									; Read sector
mov word [SectorInCache], ax				; Set cache metadata
mov byte [DriveInCache], dl
jmp .cache_hit								; Now we can use the cache

.done:

pop es
pop ds
pop edx
pop ecx
pop ebx
iret

.target_sector		dw	0x0000
.target_offset		dw	0x0000
