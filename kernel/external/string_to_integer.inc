string_to_integer:

;	Converts a numeric string into an integer
;     Input string in DS:SI (0x00 terminated)
;     Output integer in EAX

push ebx
push ecx
push edx
push si

xor ecx, ecx			; Prepare ECX

.loop:
	xor eax, eax		; Prepare EAX
	lodsb				; Get byte from string
	sub al, '0'			; Get integer value
	mov dl, byte [si]	; Check whether this is the last byte in the string
	test dl, dl			; Is the string terminated?
	jz .done			; If yes, we're done
	add ecx, eax		; Add new value to stored result
	mov eax, 10
	mul ecx				; Multiply result by 10
	mov ecx, eax		; Store result back in ECX
	jmp .loop			; Loop

.done:
	add ecx, eax		; Add last unit
	mov eax, ecx		; Move result into EAX
	pop si
	pop edx
	pop ecx
	pop ebx
	iret
