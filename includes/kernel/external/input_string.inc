input_string:

;   Gets a string from keyboard (0x00 terminated).
;     maximum string length in BX
;     buffer to save string to in ES:DI

push ax
push bx
push cx
push di

xor cx, cx
.loop:
	mov ah, 0x00
	int 0x16
	cmp al, 0x08
	je .backspace
	cmp al, 0x0D
	je .enter
	cmp cx, bx
	je .loop
	inc cx
	push 0x01
	int 0x80
	stosb
	jmp .loop
.backspace:
	cmp cx, 0
	je .loop
	mov al, 0x08
	push 0x01
	int 0x80
	dec di
	dec cx
	jmp .loop
.enter:
	mov al, 0x00
	stosb
	pop di
	pop cx
	pop bx
	pop ax
	iret
