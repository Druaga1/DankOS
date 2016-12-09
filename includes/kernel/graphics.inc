;********************************************************************************
;			Graphics driver for DankOS
;
;********************************************************************************
graphics_set_mode:
	;	IN:	AX --> Video Mode	01h: 40x25 Text
	;							03h: 80x25 Text
	;							13h: 320x200 Graphics
	
	mov ah,00h
	int 10h						;Update the graphics mode
	iret
;---------------------------------------------------------------------------
graphics_draw_rectangle:
	;	IN:	BX --> Color of rectangle
	;		CX --> X Position, Y Position
	;		DX --> Width, Height
	
	mov al,13h
	push 0x25	
	int 0x80					; Set graphics mode to 320x200 Graphics
	
	push 0x26
	int 0x80					; Draw a pixel with the color of register BL at position [CH,CL*320]
	
	iret
;---------------------------------------------------------------------------
graphics_write_pixel:
	;	IN:	DL --> Color of pixel
	;		CH --> X Position
	;		CL --> Y Position
	push es
	push di
	push ax
	
	mov     ax, 0A000h     		; Set ES to graphics screen segment
	mov     es, ax
	mul     di, ch, 320    		; Multiply Y by 320 and store in DI
	add     di, cl         		; Add the X coordinate
	mov     [es:di], dl     	; Set the pixel to the color specified
	
	mov     ax, 0003h
	int     10h					; Switch back to text
	
	pop ax
	pop di
	pop es
	iret
;---------------------------------------------------------------------------
graphics_change_color:
	;	IN:	BX --> Color
	push ax
	mov ax,bx
	mov al, 0x70
	push 0x11
	int 0x80				; Set palette and reset screen
	pop ax
	iret	
;---------------------------------------------------------------------------
%define COLOR_BLACK			00h
%define COLOR_BLUE			01h	
%define COLOR_GREEN			02h	
%define COLOR_CYAN			03h	
%define COLOR_RED			04h	
%define COLOR_MAGENTA		05h	
%define COLOR_BROWN			06h	
%define COLOR_LIGHTGRAY		07h	
%define COLOR_DARKGRAY		08h	
%define COLOR_LIGHTBLUE		09h	
%define COLOR_LIGHTGREEN	0Ah	
%define COLOR_LIGHTCYAN		0Bh	
%define COLOR_LIGHTRED		0Ch	
%define COLOR_LIGHTMAGENTA	0Dh	
%define COLOR_YELLOW		0Eh	
%define COLOR_WHITE			0Fh
;---------------------------------------------------------------------------
CURSOR_COLOR				db 00h
	
	
