;
;
;
;------------------------------------------------------------
;	IN -->	AX : Y cord
;			BX : X cord
;			DL : Pixel Color
graphics_write_pixel:
	mov cx,320
	mul cx			; multiply AX by 320 (cx value)
	add ax,bx 		; and add X
	mov di,ax
	mov dl,7
	mov [es:di],dl	; Write the pixel to video memory
	ret
;------------------------------------------------------------
;	IN -->	AX : Y cord
;			BX : X cord
;			CX : Length
;			DL : Pixel Color
;graphics_draw_hline:			;Drawing top->down
	;Check for errors before drawing
	;cmp ax, cx					;check for end
	;jc gerror_maxgreatermin		;If the current position is greater than the maximum, stop drawing.

graphics_draw_hline_loop:
	push ax
	mov ah, 00h
	mov ax, 13h ; AH=0 (Change video mode), AL=13h (Mode)
	int 10h 	; Video BIOS interrupt
	pop ax
	push cx
	call graphics_write_pixel	;Place the pixel
	pop cx
	inc bx						;move down to next point
	cmp bx, cx					;check for end
	jnz graphics_draw_hline_loop;If not finished, loop
	mov ah, 00h
	mov ax,3	; AH=0 (Change video mode)
	int 10h		; Video BIOS interrupt
	iret
;------------------------------------------------------------
;	IN -->	AX : Y cord
;			BX : X cord
;			CX : Length
;			DL : Pixel Color
;graphics_draw_vline:			;Drawing top->down
	;Check for errors before drawing
	;cmp ax, cx					;check for end
	;jc gerror_maxgreatermin		;If the current position is greater than the maximum, stop drawing.

graphics_draw_vline_loop:	
	push cx
	call graphics_write_pixel	;Place the pixel
	pop cx
	inc ax						;move down to next point
	cmp ax, cx					;check for end
	jnz graphics_draw_vline_loop;If not finished, loop
	iret
;------------------------------------------------------------
vertical_error	DB	'Error in drawing vertical line: \n\0'
maximum_error	DB	'Current position is greater than length set'
	
