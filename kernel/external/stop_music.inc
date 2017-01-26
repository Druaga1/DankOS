stop_music:

push cx
push es

mov cx, KernelSpace
mov es, cx

; Stop speaker

push 0x1E
int 0x80

mov byte [es:MusicPlayingFlag], 0

pop es
pop cx
iret
