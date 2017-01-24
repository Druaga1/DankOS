upper_to_lowercase:

;   Transform string to LOWERCASE (0x00 terminated).
;     string in ES:EDI

push ax
push edi

.loop:
	mov al, byte [es:edi]	; Byte from EDI
	cmp al, 0x40
	jg .convert
	cmp al, 0x00
	je .done
.no_convert:
	inc edi
	jmp .loop

.convert:
	cmp al, 0x5A
	jg .no_convert
	add al, 0x20
	a32 o32 stosb
	jmp .loop

.done:
	pop edi
	pop ax
	iret
