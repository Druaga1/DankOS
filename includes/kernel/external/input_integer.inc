input_integer:

;   Gets a 32-bit integer from keyboard.
;     returns number in EAX

push ebx
push ecx
push edx
push si
push di
push ds
push es

mov ax, 0x9000
mov ds, ax
mov es, ax

xor cx, cx
xor dx, dx
mov di, .buffer
.loop:
	mov ah, 0x00
	int 0x16
	cmp al, 0x08
	je .backspace
	cmp al, 0x0D
	je .enter
	cmp al, '0'
	jl .loop
	cmp al, '9'
	jg .loop
	cmp cl, 10
	je .loop
	inc cl
	push 0x01
	int 0x80
	sub al, '0'
	stosb
	jmp .loop
.backspace:
	cmp cl, 0
	je .loop
	mov al, 0x08
	push 0x01
	int 0x80
	dec di
	dec cl
	jmp .loop
.enter:
	mov si, .buffer
	xor eax, eax
.ascii_to_integer:
	cmp cl, 0x00
	je .done
	mov edx, 10
	mul edx
	xor edx, edx
	mov dl, byte [si]
	inc si
	add eax, edx
	dec cl
	jmp .ascii_to_integer
.done:
	pop es
	pop ds
	pop di
	pop si
	pop edx
	pop ecx
	pop ebx
	iret
.buffer times 10 db 0x00
