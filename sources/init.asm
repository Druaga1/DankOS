; ** init.bin **

bits 16			; 16-bit real mode

mov si, msg		; Print message
int 0x42

int 0x58		; Pause

int 0x40		; Quit

msg db 0x0A, 'init.bin has been started.'
	db 0x0A, 'This is just a stub, the shell is not ready yet.'
	db 0x0A, 'You can change the init.bin file to load a different program.'
	db 0x0A, 0x0A, 'Press a key to quit init...', 0x00
