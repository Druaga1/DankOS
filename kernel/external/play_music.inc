play_music:

; IN: DS:ESI --> Start address of the music file
;     DL --> Repeat flag

push ax
push es

mov ax, KernelSpace
mov es, ax

mov ax, ds
mov word [es:MusicSegment], ax
mov dword [es:MusicStartAddress], esi
mov dword [es:MusicCurrentNote], esi
mov byte [es:MusicRepeatFlag], dl
mov byte [es:MusicPlayingFlag], 1

pop es
pop ax
iret
