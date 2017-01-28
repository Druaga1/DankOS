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

clc										; Clear carry for int 0x13 because some BIOSes may not clear it on success

int 0x13								; Call int 0x13

.done:

pop dx									; Restore all GPRs
pop cx
pop bx
pop ax
ret										; Exit routine


.absolute_sector		db 0x00
.absolute_head			db 0x00
.absolute_track			db 0x00
