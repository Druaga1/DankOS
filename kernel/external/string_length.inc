string_length:

;   Returns the length of a string (0x00 terminated).
;     string in DS:ESI
;     returns length in ECX

push ax
push esi

xor ecx, ecx
.loop:
	a32 o32 lodsb	; Byte from ESI
	test al, al		; Is 0?
	jz .done
	inc ecx
	jmp .loop

.done:
pop esi
pop ax
iret
