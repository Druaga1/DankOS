compare_strings:

;   Compares 2 strings (0x00 terminated).
;     strings in DS:ESI and ES:EDI
;     returns DL = 0x01 if equal 0x00 if not

push ax
push esi
push edi
.loop:
	a32 o32 lodsb
	mov ah, byte [es:edi]
	inc edi
	cmp al, ah
	jne .not_equal
	test al, al
	jz .equal
	jmp .loop

.not_equal:
	xor dl, dl
	jmp .done

.equal:
	mov dl, 0x01

.done:
	pop edi
	pop esi
	pop ax
	iret
