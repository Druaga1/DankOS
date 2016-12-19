floppy_read_sector:

; ********************************************************************
;     Reads a floppy sector with an LBA address (internal routine)
; ********************************************************************

; IN:
; AX = LBA sector to load
; DL = Drive number
; ES = Buffer segment
; BX = Buffer offset

; OUT:
; Carry if error

push ax									; Save all GPRs
push bx									; Prepare entering routine
push cx
push dx

push bx									; Save target buffer in stack
push dx									; Save drive number in stack

; LBA to CHS

xor dx, dx								; XOR DX for division
mov bx, 18								; Divide LBA / Sectors per track (18 on 1.44 floppy)
div bx
inc dl									; Adjust for sector 0
mov byte [.absolute_sector], dl			; Save sector
xor dx, dx								; XOR DX for division
mov bx, 2
div bx									; Divide / Number of heads (2)
mov byte [.absolute_head], dl			; Save head
mov byte [.absolute_track], al			; Save track

pop dx									; Restore drive number from stack
pop bx									; Restore target buffer from stack

; Prepare registers for BIOS int 0x13

mov ah, 0x02							; Read sector function
mov al, 1								; Read 1 sector
mov ch, byte [.absolute_track]			; Use data we calculated
mov cl, byte [.absolute_sector]
mov dh, byte [.absolute_head]

;mov byte [.tries_counter], 5			  Allow for 5 reading tries
										; I am doing this ugly thing here and not using CX
										; because it is busy, so this is actually a nicer solution :)
										; add: To speed things up, the number of tries is hard coded down below

.retry:

clc										; Clear carry for int 0x13 because some BIOSes may not clear it on success

int 0x13								; Call int 0x13

jnc .done								; If there wasn't an error, we're done
cmp byte [.tries_counter], 0x0000		; If the counter has reached zero
je .done								; just exit the loop and keep the carry flag
dec byte [.tries_counter]				; Otherwise decrement counter
jmp .retry								; and retry

.done:

pop dx									; Restore all GPRs
pop cx
pop bx
pop ax
ret										; Exit routine


.absolute_sector		db 0x00
.absolute_head			db 0x00
.absolute_track			db 0x00
.tries_counter			db 5


floppy_read_sectors:

; **********************************************************
;     Reads multiple LBA addressed sectors from a floppy
; **********************************************************

; IN:
; AX = LBA starting sector
; DL = Drive number
; ES = Buffer segment
; BX = Buffer offset
; CX = Sectors count

push ax									; Save GPRs
push bx
push cx
push dx
push ds									; Save DS

mov dx, 0x9000							; Point DS to kernel space
mov ds, dx

.loop:

call floppy_read_sector					; Read sector
jc .done								; If carry, exit routine

inc ax									; Increment sector
add bx, 512								; Add 512 to the buffer

loop .loop								; Loop!

.done:
pop ds
pop dx
pop cx									; Restore GPRs
pop bx
pop ax
iret									; Exit routine


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

mov cx, 0x9000			; Prepare DS and ES
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

cmp byte [floppy_cache_status], 0xFF
jne .cache_miss
cmp ax, word [floppy_sector_in_cache]
jne .cache_miss
cmp dl, byte [floppy_drive_in_cache]
jne .cache_miss

.cache_hit:

; The sector is already loaded in the cache, read the byte and return

mov bx, word [.target_offset]
pop eax							; Restore the rest of EAX
add bx, floppy_cache
mov al, byte [bx]
jmp .done

.cache_miss:

; The cache is not useful, we need to fetch the sector from the drive

mov byte [floppy_cache_status], 0xFF		; Flag cache as enabled
mov ax, word [.target_sector]
mov bx, floppy_cache						; Target buffer is cache
mov cx, 1									; Read 1 sector
push 0x23
int 0x80									; Read sector
mov word [floppy_sector_in_cache], ax		; Set cache metadata
mov byte [floppy_drive_in_cache], dl
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


; Global variables

floppy_cache_status			db 0x00
floppy_drive_in_cache		db 0x00
floppy_sector_in_cache		dw 0x0000
floppy_cache				times 512 db 0x00
