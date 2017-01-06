;----------------------------------------------------------------------
;
;    IN: DL --> Color of pixel
;        CH --> X Position
;        CL --> Y Position
;        BX --> Length
draw_horizontal_line:
	push bx
	push cx
	push dx
.loop:
	test bx, bx
	jz .done
	dec bx
	inc ch
	push 0x81
	int 0x80
    jmp .loop
.done:
	pop dx
	pop cx
	pop bx
	iret
;----------------------------------------------------------------------
;
;    IN: DL --> Color of pixel
;        CH --> X Position
;        CL --> Y Position
;        BX --> Length
draw_vertical_line:
	push bx
	push cx
	push dx
.loop:
	test bx, bx
	jz .done
	dec bx
	inc cl
	push 0x81
	int 0x80
    jmp .loop
.done:
	pop dx
	pop cx
	pop bx
	iret
;----------------------------------------------------------------------