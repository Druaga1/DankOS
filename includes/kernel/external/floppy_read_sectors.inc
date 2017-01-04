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

.loop:

call floppy_read_sector					; Read sector
jc .done								; If carry, exit routine

inc ax									; Increment sector
add bx, 512								; Add 512 to the buffer

loop .loop								; Loop!

.done:
pop dx
pop cx									; Restore GPRs
pop bx
pop ax
iret									; Exit routine
