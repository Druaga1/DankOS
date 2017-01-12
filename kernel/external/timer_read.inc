timer_read:

; IN = Nothing
; OUT = CH = hours
;       CL = minutes
;       DH = seconds
;		DL = DST

;  /// NOTE: the problem with the time is probably due to the fact that it's returned in BCD format
;            We need to add a conversion routine in this function (done by me)

	push ax

	.retry:
	clc					; Clear carry flag
	mov ah, 0x02		; Read RTC (Hours, Minutes, Seconds)
	int 0x1A			; RTC Call
	jc .retry			; If error, retry

	mov al, ch			; Convert hours
	call .bcd_to_integer
	mov ch, al
	mov al, cl			; Minutes
	call .bcd_to_integer
	mov cl, al
	mov al, dh			; And seconds
	call .bcd_to_integer
	mov dh, al

	pop ax
	iret

	.bcd_to_integer:

;	AL = BCD number, returns integer in AL

	push cx					; Save CX
	push ax					; Save AX
	and al, 11110000b		; Extract the high nibble of AL
	shr al, 4				; Shift it right by 4
	mov cl, 10				; Multiply by 10
	mul cl
	pop cx					; Restore AX in CX
	and cl, 00001111b		; Extract the low nibble
	add al, cl				; And add it back to AL
	pop cx					; Restore CX
	ret
