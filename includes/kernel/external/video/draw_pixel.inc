;----------------------------------------------------------------------
;
;	IN:	DL --> Color of pixel
;		BX --> X Position
;		CL --> Y Position

draw_pixel:
	push ax
	push bx
	push cx
	push dx
	push es
	push di

	push dx
	
	mov     ax, FrameBuffer		; Set ES to graphics (frame buffer) screen segment
	mov     es, ax

	mov 	ax, 320    			; Multiply Y by 320 and store in DI
	xor		ch, ch
	mul		cx
	mov		di, ax

	add     di, bx         		; Add the X coordinate

	pop dx

	mov     byte [es:di], dl   	; Set the pixel to the color specified

	pop di
	pop es
	pop dx
	pop cx
	pop bx
	pop ax
	iret
