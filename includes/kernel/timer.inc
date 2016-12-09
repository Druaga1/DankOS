timer_read:
	mov ah, 0x02		; Read RTC (Hours, Minutes, Seconds)
	int 0x1A			; RTC Call
	iret

timer_print_time:

	push eax
	push bx
	push cx
	push dx
	
	push 0x20				;Read RTC
	int 0x80
	
	xor eax, eax

	mov al, ch
	push 0x06			; Print out the hours
	int 0x80
	
	mov al, ':'
	push 0x01			; Print divider
	int 0x80
	
	mov al, cl
	push 0x06			; Print out the minutes
	int 0x80
	
	mov al, ':'
	push 0x01			; Print divider
	int 0x80
	
	mov al, dh
	push 0x06			; Print out the seconds
	int 0x80

	pop dx
	pop cx
	pop bx
	pop eax
	iret
