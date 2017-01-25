play_music:

; IN: DS:ESI --> Start address of the music file
;     DL --> Repeat flag

push cx
push es

mov cx, KernelSpace
mov es, cx

mov cx, ds
mov word [es:MusicSegment], cx
mov dword [es:MusicStartAddress], esi
mov dword [es:MusicCurrentNote], esi
mov byte [es:MusicRepeatFlag], dl

; Play starting note

mov cx, word [ds:esi+1]
push 0x22
int 0x80

mov byte [es:MusicPlayingFlag], 1

pop es
pop cx
iret
