clear_frame_buffer:

push ax
push ecx
push edi
push es

xor ax, ax				; Clear frame buffer
mov es, ax

mov edi, FrameBuffer

xor al, al
xor ecx, ecx
not cx

a32 o32 rep stosb

pop es
pop edi
pop ecx
pop ax
iret
