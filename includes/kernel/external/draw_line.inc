;----------------------------------------------------------------------
;
;    IN: DL --> Color of pixel
;        CH --> X Starting position
;        CL --> Y Starting position
;        BH --> X Ending position
;        BL --> Y Ending position
draw_line:
	push ax
	push bx
	push cx
	push dx
	push ds

; Point DS to kernel

	mov ax, 0x9000
	mov ds, ax


; Using this algorithm from https://en.wikipedia.org/wiki/Bresenham's_line_algorithm

;	plotLine(x0,y0, x1,y1)
;	  dx = x1 - x0
;	  dy = y1 - y0
;	  D = 2*dy - dx
;	  y = y0

;	  for x from x0 to x1
;	    plot(x,y)
;	    if D > 0
;	       y = y + 1
;	       D = D - dx
;	    end if
;	    D = D + dy

; Save colour

	mov byte [.Colour], dl

; Get "dx"

	push cx
	push bx
	xor bl, bl
	shr bx, 8
	mov word [.dx], bx
	xor cl, cl
	shr cx, 8
	sub word [.dx], cx
	pop bx
	pop cx

; Get "dy"

	push cx
	push bx
	xor bh, bh
	mov word [.dy], bx
	xor ch, ch
	sub word [.dy], cx
	pop bx
	pop cx

; Get "D"

	push cx
	push bx
	mov ax, word [.dy]
	mov bx, 2
	mul bx
	sub ax, word [.dx]
	mov word [.D], ax
	pop bx
	pop cx

; For x from x0 to x1

	.loop:
	cmp ch, bh
	je .done
	inc ch


;	    plot(x,y)

	mov dl, byte [.Colour]
	push 0x81
	int 0x80


;	    if D > 0

	cmp word [.D], 0x0000
	jg .do_stuff
	.stuff_done:

;	    end if
;	    D = D + dy

	mov ax, word [.dy]
	add word [.D], ax
	jmp .loop

;	       y = y + 1
;	       D = D - dx

	.do_stuff:
	inc cl
	mov ax, word [.D]
	sub ax, word [.dx]
	mov word [.D], ax
	jmp .stuff_done


.done:
	pop ds
	pop dx
	pop cx
	pop bx
	pop ax
	iret


.Colour	db	0x00
.D		dw	0x0000
.dx		dw	0x0000
.dy		dw	0x0000
