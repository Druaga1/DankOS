; ***********************************
;     Returns the current palette
; ***********************************

; OUT: AH = Char palette
;      AL = Cursor palette

get_current_palette:

push ds

mov ax, KernelSpace
mov ds, ax

mov ah, byte [video_char_attributes]
mov al, byte [video_cursor_attributes]

pop ds

iret
