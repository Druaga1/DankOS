floppy_write_sectors:

; *********************************************************
;     Writes multiple LBA addressed sectors to a floppy
; *********************************************************

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

call floppy_write_sector				; Write sector
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
