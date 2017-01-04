string_copy:

;   Copies a string
;     Input string in DS:SI, copies into ES:DI (0x00 terminated)

push ax
push si
push di

.loop:
	lodsb
	stosb
	test al, al
	jz .done
	jmp .loop

.done:
	pop di
	pop si
	pop ax
	iret
