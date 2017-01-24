lower_to_uppercase:

;   Transform string to UPPERCASE (0x00 terminated).
;     string in ES:EDI

push ax
push edi

.loop:
	mov al, byte [es:edi]	; Byte from EDI
	cmp al, 0x60
	jg .convert
	cmp al, 0x00
	je .done
.no_convert:
	inc edi
	jmp .loop

.convert:
	cmp al, 0x7A
	jg .no_convert
	sub al, 0x20
	a32 o32 stosb
	jmp .loop

.done:
	pop edi
	pop ax
	iret
