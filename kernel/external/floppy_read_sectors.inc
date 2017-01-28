floppy_read_sectors:

; **********************************************************
;     Reads multiple LBA addressed sectors from a floppy
; **********************************************************

; IN:
; AX = LBA starting sector
; DL = Drive number
; ES = Buffer segment
; EBX = Buffer offset
; CX = Sectors count

push eax									; Save GPRs
push ebx
push ecx
push edx
push esi
push edi
push ds

mov si, KernelSpace
mov ds, si

.loop:

push es
push ebx

mov si, KernelSpace
mov es, si

mov byte [CacheStatus], 0xFF
mov byte [DriveInCache], dl
mov word [SectorInCache], ax
mov bx, DiskCache

call floppy_read_sector					; Read sector

pop ebx
pop es

jc .done								; If carry, exit routine

mov edi, ebx
mov esi, DiskCache
push ecx
mov ecx, 512
a32 o32 rep movsb
pop ecx

inc ax									; Increment sector
add ebx, 512							; Increment buffer

loop .loop								; Loop!

.done:
pop ds
pop edi
pop esi
pop edx
pop ecx									; Restore GPRs
pop ebx
pop eax
iret									; Exit routine
