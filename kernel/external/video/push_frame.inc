push_frame:

push ds
push es
push esi
push edi
push ecx

xor cx, cx
mov ds, cx
mov cx, 0xA000
mov es, cx

mov esi, FrameBuffer
xor edi, edi

xor ecx, ecx
not cx

a32 o32 rep movsb

pop ecx
pop edi
pop esi
pop es
pop ds
iret
