;----------------------------------------------------------------------
;
;	IN:	DL --> Color of pixel
;		CH --> X Position
;		CL --> Y Position

draw_pixel:
	push ax
	push bx
	push cx
	push dx
	push es
	push di

	push dx
	
	mov     ax, 0xA000     		; Set ES to graphics screen segment
	mov     es, ax

	push cx

	mov 	ax, 320    			; Multiply Y by 320 and store in DI
	xor		ch, ch
	mul		cx
	mov		di, ax

	pop cx

	xor		cl, cl
	shr		cx, 8
	add     di, cx         		; Add the X coordinate

	pop dx

	mov     byte [es:di], dl   	; Set the pixel to the color specified

	pop di
	pop es
	pop dx
	pop cx
	pop bx
	pop ax
	iret


