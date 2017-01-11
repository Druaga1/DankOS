set_current_drive:

; ******************************
;     Sets the current drive
; ******************************

; IN: DL = drive number

; OUT: nothing

push ax
push ds

mov ax, KernelSpace
mov ds, ax

mov byte [CurrentDrive], dl			; Set up the current drive variable
mov word [CurrentDirectoryPath], 0x002F	; Set the current directory path

call fat_load_root					; Load the root directory

pop ds
pop ax
iret
