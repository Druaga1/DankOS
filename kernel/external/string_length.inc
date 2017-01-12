string_length:

;   Returns the length of a string (0x00 terminated).
;     string in DS:SI
;     returns length in CX

push ax
push si

xor cx, cx
.loop:
	lodsb			; Byte from SI
	test al, al		; Is 0?
	jz .done
	inc cx
	jmp .loop

.done:
pop si
pop ax
iret
