string_end:

;   Get to the end of the string, ES:EDI will point to the terminator
;     ES:EDI string

push ax

.loop:
	mov al, byte [es:edi]
	test al, al
	jz .done
	inc edi
	jmp .loop

.done:
	pop ax
	iret
