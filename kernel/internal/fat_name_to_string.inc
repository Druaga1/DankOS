fat_name_to_string:

;	Trasform FAT file name (ex. 'PHOTO   IMG') to a readable format (ex. 'PHOTO.IMG').
;     Input string in DS:SI (0x00 terminated)
;     Output string in ES:DI (0x00 terminated)
;       THE BUFFER SIZE SHOULD ALWAYS BE 13 BYTES, SINCE THE FILE NAME IS 12 + 0x00

push ax
push cx
push si
push di

mov cx, 8		; The name is 8 characters max
.name_loop:
	lodsb
	cmp al, ' '		; Check for space
	je .skip_padding_loop
	stosb
	loop .name_loop

.skip_padding_loop:
	mov al, byte [ds:si]
	test al, al
	jz .done		; No extension
	cmp al, ' '
	jne .extension
	inc si
	jmp .skip_padding_loop

.extension:
	mov al, '.'
	stosb

.extension_loop:
	lodsb
	cmp al, ' '
	je .done
	test al, al
	jz .done
	stosb
	jmp .extension_loop

.done:
	xor al, al		; Add terminator
	stosb

	pop di
	pop si
	pop cx
	pop ax
	ret
