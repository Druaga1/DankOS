compare_strings:

;   Compares 2 strings (0x00 terminated).
;     strings in DS:SI and ES:DI
;     returns DL = 0x01 if equal 0x00 if not

push ax
push si
push di
.loop:
	lodsb
	mov ah, byte [es:di]
	inc di
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
	pop di
	pop si
	pop ax
	iret
