string_copy:

;   Copies a string
;     Input string in DS:ESI, copies into ES:EDI (0x00 terminated)

push ax
push esi
push edi

.loop:
	a32 o32 lodsb
	a32 o32 stosb
	test al, al
	jz .done
	jmp .loop

.done:
	pop edi
	pop esi
	pop ax
	iret
