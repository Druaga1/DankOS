clear_screen:

push ax
push cx
push di
push es

mov ax, 0xA000			; Point ES to the lower part of video memory
mov es, ax

xor di, di

xor al, al
xor cx, cx
not cx

rep stosb

mov ax, 0xB000			; Point ES to the upper part of video memory
mov es, ax

xor di, di

xor al, al
xor cx, cx
not cx

rep stosb

mov ax, 0x0050			; Point ES to the frame buffer
mov es, ax

xor di, di

xor al, al
xor cx, cx
not cx

rep stosb

pop es
pop di
pop cx
pop ax
iret
