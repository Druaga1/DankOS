; ** Print command **

timer_read:
	mov ah, 0x02		; Read RTC (Hours, Minutes, Seconds)
	int 0x1A			; RTC Call
	iret

timer_print_time:
	
	push 0x20				;Read RTC
	int 0x80
	
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
	
	push 0x10
	int 0x80

	push 0x03
	int 0x80			; New line
	iret