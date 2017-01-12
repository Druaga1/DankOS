string_to_fat_name:

;	Trasform readable string (ex. 'photo.img') to FAT file name (ex. 'PHOTO   IMG').
;     Input string in DS:SI (0x00 terminated)
;     Output string in ES:DI (0x00 terminated)
;       THE BUFFER SIZE SHOULD ALWAYS BE 12 BYTES, SINCE THE FILE NAME IS 11 + 0x00

push ax
push cx
push dx
push si
push di

push di
push es

mov ax, KernelSpace
mov es, ax

mov di, .dot_dot_entry
push 0x08
int 0x80

test dl, dl
jz .ignore

mov di, .dot_entry
push 0x08
int 0x80

test dl, dl
jz .ignore

pop es
pop di

mov cl, 11

.clear_buffer:
	cmp cl, 0
	je .main
	mov al, ' '
	stosb
	dec cl
	jmp .clear_buffer

.main:
	pop di
	push di
	mov cl, 11

.loop:
	lodsb			; Byte from SI
	cmp al, '.'		; Is '.'?
	je .pad_and_extension
	cmp al, 0x00	; Is terminator?
	je .pad_and_convert
	cmp cl, 3		; Too may characters?
	je .convert		; Quit
	stosb			; Save in DI
	dec cl
	jmp .loop

.pad_and_extension:
	sub cl, 3		; Sub extension size from padding
.loop1:
	cmp cl, 0
	je .put_extension
	mov al, ' '		; Pad with spaces
	dec cl
	stosb
	jmp .loop1
.put_extension:
	mov cl, 3
.loop2:
	cmp cl, 0
	je .convert
	lodsb			; Byte from SI
	cmp al, 0x00	; Is 0x00?
	je .pad_and_convert
	stosb			; Store in DI
	dec cl
	jmp .loop2

.pad_and_convert:
	cmp cl, 0
	je .convert
	mov al, ' '		; Pad with spaces
	stosb
	dec cl
	jmp .pad_and_convert

.convert:
	pop di			; Reset DI
	push di
	add di, 11
	mov byte [es:di], 0x00	; Add 0x00 terminator
	sub di, 11
	push 0x15
	int 0x80		; Lower to uppercase

.done:
	pop di
	pop si
	pop dx
	pop cx
	pop ax
	ret

.ignore:

	pop es
	pop di

mov cx, 11

.clear_buffer1:
	mov al, ' '
	stosb
	loop .clear_buffer1

	pop di
	push di

	.copy_string:
	lodsb
	test al, al
	jz .done
	stosb
	jmp .copy_string

.dot_dot_entry		db	'..', 0x00
.dot_entry			db	'.', 0x00
