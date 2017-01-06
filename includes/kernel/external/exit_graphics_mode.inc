exit_graphics_mode:
    push ax
	push bx
	push cx
	push dx

	mov al, 0x03
    mov ah,00                ;subfunction 0
    int 10h                    ;call graphics interrupt


	; Prepare the screen

	mov ax, 0x1003
	mov bl, 0x00
	xor bh, bh
	int 0x10				; Disable blinking with BIOS

	mov dh, 24
	mov dl, 80
	mov bh, 0x00
	mov ah, 0x02
	int 0x10				; Disable BIOS cursor

	mov ah, 0x02
	mov al, 0x70
	push 0x11
	int 0x80				; Set palette and reset screen
	push 0x0A
	int 0x80


	pop dx
	pop cx
	pop bx
    pop ax
    iret
