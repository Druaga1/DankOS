input_string:

;   Gets a string from keyboard (0x00 terminated).
;     maximum string length in EBX
;     buffer to save string to in EDI

push ax
push ebx
push ecx
push edi

xor ecx, ecx
.loop:
	push 0x1C
	int 0x80
	cmp al, 0x08
	je .backspace
	cmp al, 0x0D
	je .enter
	test al, al
	jz .loop
	cmp ecx, ebx
	je .loop
	inc ecx
	push 0x01
	int 0x80
	a32 o32 stosb
	jmp .loop
.backspace:
	test ecx, ecx
	jz .loop
	mov al, 0x08
	push 0x01
	int 0x80
	dec edi
	dec ecx
	jmp .loop
.enter:
	xor al, al
	a32 o32 stosb
	pop edi
	pop ecx
	pop ebx
	pop ax
	iret
