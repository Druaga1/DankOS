; **********************************************
;     Draws a sprite from a formatted sprite
; **********************************************

; IN: DS:SI --> Location of the sprite data
;     BX --> X location of where to print it
;     CL --> Y location of where to print it

; SPRITE DATA FORMATTING:

;	Sprite_X_Length		word
;	Sprite_Y_Length		byte
;	Sprite_Data:
;		A sequence of bytes indicating the pixel colours

draw_sprite:

push ax
push bx
push cx
push dx
push si
push es

mov ax, KernelSpace
mov es, ax

mov word [es:.OriginalBX], bx
lodsw
mov word [es:.SpriteX], ax
add word [es:.SpriteX], bx
lodsb
mov byte [es:.SpriteY], al
add byte [es:.SpriteY], cl

.loop:
lodsb
mov dl, al
cmp bx, word [es:.SpriteX]
je .next_row
push 0x81
int 0x80
inc bx
jmp .loop

.next_row:
mov bx, word [es:.OriginalBX]
inc cl
cmp cl, byte [es:.SpriteY]
je .done
push 0x81
int 0x80
inc bx
jmp .loop

.done:

pop es
pop si
pop dx
pop cx
pop bx
pop ax

iret





.OriginalBX		dw	0x0000
.SpriteX		dw	0x0000
.SpriteY		db	0x00
