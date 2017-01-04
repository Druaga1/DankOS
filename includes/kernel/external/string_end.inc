string_end:

;   Get to the end of the string, DI will point to the terminator
;     ES:DI string

push ax

.loop:
	mov al, byte [es:di]
	test al, al
	jz .done
	inc di
	jmp .loop

.done:
	pop ax
	iret
