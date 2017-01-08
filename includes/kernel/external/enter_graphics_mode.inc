enter_graphics_mode:
    push ax
	mov al, 0x13
    mov ah,00                ;subfunction 0
    int 10h                    ;call graphics interrupt
	push 0x85				; Clear screen
	int 0x80
    pop ax
    iret
