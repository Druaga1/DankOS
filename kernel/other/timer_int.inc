; ***********************************************
;     Executes timer-related kernel functions
; ***********************************************

timer_int:

push eax
push gs

mov ax, KernelSpace
mov gs, ax

inc dword [gs:ClockTicks]

cmp byte [gs:MusicPlayingFlag], 0x01
je .playmusic






.done:

pop gs
pop eax

iret



.playmusic:

push cx
push esi
push ds

mov cx, word [gs:MusicSegment]
mov ds, cx

mov esi, dword [gs:MusicCurrentNote]

xor cx, cx
mov cl, byte [ds:esi]

test cl, cl
jz .music_end

cmp byte [gs:NoteLengthCounter], cl
je .next_note

inc byte [gs:NoteLengthCounter]

.playmusic_done:

pop ds
pop esi
pop cx
jmp .done


.next_note:

add dword [gs:MusicCurrentNote], 3
mov byte [gs:NoteLengthCounter], 0

mov esi, dword [gs:MusicCurrentNote]

mov cx, word [ds:esi+1]
test cx, cx
jz .music_pause

push 0x22
int 0x80

jmp .playmusic_done


.music_end:

push 0x1E
int 0x80

mov byte [gs:MusicPlayingFlag], 0x00
mov byte [gs:NoteLengthCounter], 0
jmp .playmusic_done


.music_pause:

push 0x1E
int 0x80

jmp .playmusic_done
















