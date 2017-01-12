upper_to_lowercase:

;   Transform string to LOWERCASE (0x00 terminated).
;     string in ES:DI

push ax
push di

.loop:
	mov al, byte [es:di]	; Byte from DI
	cmp al, 0x40
	jg .convert
	cmp al, 0x00
	je .done
.no_convert:
	inc di
	jmp .loop

.convert:
	cmp al, 0x5A
	jg .no_convert
	add al, 0x20
	stosb
	jmp .loop

.done:
	pop di
	pop ax
	iret
