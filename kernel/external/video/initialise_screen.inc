; *****************************************************************
;     Clears the screen (text mode) to the current text palette
; *****************************************************************

initialise_screen:

push ax
push cx
push di
push ds
push es

mov ax, 0xB800
mov es, ax
mov ax, KernelSpace
mov ds, ax

xor di, di								; Point to video memory

; Draw prompt (at 0,0)

mov ah, byte [CursorAttributes]	; Get cursor attributes
mov al, ' '
stosw

; Fill the screen with spaces

mov ah, byte [CharAttributes]	; Get char attributes
mov al, ' '

mov cx, 0x07CF
rep stosw

; Set up the variables

mov word [CursorLocation], 0x0000
mov byte [CursorStatus], 0x01

pop es
pop ds
pop di
pop cx
pop ax
iret
