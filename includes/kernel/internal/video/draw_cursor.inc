; ************************
;     Draws the cursor
; ************************

draw_cursor:

push ax
push di
push es
push ds

mov ax, 0xB800
mov es, ax
mov ax, KernelSpace
mov ds, ax

mov di, word [CursorLocation]
mov al, byte [es:di]
mov ah, byte [CursorAttributes]

stosw

pop ds
pop es
pop di
pop ax

ret
