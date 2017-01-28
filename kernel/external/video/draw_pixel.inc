;----------------------------------------------------------------------
;
;	IN:	DL --> Color of pixel
;		BX --> X Position
;		CL --> Y Position

draw_pixel:
	push eax
	push ebx
	push ecx
	push edx
	push es
	push edi

	push dx
	
	xor     eax, eax			; Point ES to a flat segment
	mov     es, ax

	mov		edi, FrameBuffer	; Point EDI to frame buffer

	mov 	ax, 320    			; Multiply Y by 320
	xor		ch, ch
	mul		cx
	add		edi, eax			; Add to frame buffer location

	and		ebx, 0x0000FFFF

	add     edi, ebx       		; Add the X coordinate

	pop dx

	mov     byte [es:edi], dl  	; Set the pixel to the color specified

	pop edi
	pop es
	pop edx
	pop ecx
	pop ebx
	pop eax
	iret
