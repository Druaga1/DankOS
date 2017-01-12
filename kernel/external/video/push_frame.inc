push_frame:

push ds
push es
push ax
push si
push di
push cx

mov ax, FrameBuffer
mov ds, ax
mov ax, 0xA000
mov es, ax

xor si, si
xor di, di

xor cx, cx
not cx

rep movsb

pop cx
pop di
pop si
pop ax
pop es
pop ds
iret
