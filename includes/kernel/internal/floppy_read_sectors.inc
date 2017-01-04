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
