video_put_char:

push ax
push bx
push cx
push dx
push es
push fs

cmp al, 0xFF					; Is 0xFF (null)?
je .done						; Ignore

test al, al						; Is 0x00 (null)?
jz .done						; Ignore

mov bx, 0xB800							; Point ES to video memory
mov es, bx

mov bx, 0x9000							; Point FS to kernel space
mov fs, bx

cmp al, 0x0A					; Is 0x0A?
je .next_line

cmp al, 0x08					; Is 0x08?
je .backspace

cmp al, 0x09					; Is 0x09?
je .tab

mov bx, word [fs:video_cursor_location]
inc bx
mov ah, byte [fs:video_char_attributes]
mov byte [es:bx], ah						; Attributes first to avoid 'ghosting'
dec bx
mov byte [es:bx], al							; Print character
inc bx
inc bx
cmp bx, 0x0FA0						; End of video memory?
je .scroll_screen_down
mov word [fs:video_cursor_location], bx
cmp byte [fs:video_cursor_enabled], 1	; Check if cursor is enabled
jne .done
call video_draw_cursor				; Draw cursor
jmp .done

.tab:
push 0x0D
int 0x80								; Get cursor coordinates

xor al, al
shr ax, 8
mov bx, 8								; Divide X by 8 to get the cell number
xor dx, dx
div bx

mov cx, 8
sub cx, dx								; Retrieve %
mov al, ' '								; Fill space with spaces

.add_tab_loop:
push 0x01
int 0x80
loop .add_tab_loop

jmp .done

.next_line:
push 0x0D
int 0x80								; Get cursor coordinates
xor ah, ah								; Set X to 0
cmp al, 24								; Last line?
je .scroll_screen_down					; Scroll screen
inc al									; Y + 1
push 0x0E
int 0x80								; Move cursor!
jmp .done								; Done

.backspace:
cmp byte [fs:video_cursor_enabled], 1
jne .back_no_cursor
call video_clear_cursor
.back_no_cursor:
sub word [fs:video_cursor_location], 2
cmp byte [fs:video_cursor_enabled], 1
jne .done
call video_draw_cursor
jmp .done

.scroll_screen_down:
call video_clear_cursor			; Destroy cursor
mov bx, 160						; Next line

.scroll_down_loop:
cmp bx, 0x0FA0					; Last char?
je .clear_line
mov ax, word [es:bx]			; Get word
mov word [es:bx-160], ax		; Copy to previous line
inc bx
inc bx
jmp .scroll_down_loop

.clear_line:
sub bx, 160
mov word [fs:video_cursor_location], bx	; New cursor location
mov al, byte [fs:video_char_attributes]

.clear_line_loop:

cmp bx, 0x0FA0
je .clear_line_done
mov byte [es:bx], ' '
inc bx
mov byte [es:bx], al
inc bx
jmp .clear_line_loop

.clear_line_done:

cmp byte [fs:video_cursor_enabled], 1
jne .done
call video_draw_cursor			; Restore cursor

.done:
pop fs
pop es
pop dx
pop cx
pop bx
pop ax
iret

.tab_loop_flag					db 0x00

video_cursor_location			dw 0x0000
video_char_attributes			db 0x07
video_cursor_attributes			db 0x70
video_cursor_enabled			db 0x01


video_print_string:

push ax
push si

.loop:

lodsb							; Byte from SI
cmp al, 0x00					; Is 0x00?
je .done						; If yes, done
push 0x01
int 0x80
jmp .loop						; Loop

.done:

pop si
pop ax
iret

video_center_print_string:

push ax
push bx
push cx
push dx

push 0x09
int 0x80				; Find length of string

mov ax, 80				; 80 - length
sub ax, cx
xor dx, dx
mov bx, 2
div bx					; divide / 2
mov dx, ax
push 0x0D
int 0x80
mov ah, dl
push 0x0E
int 0x80
push 0x02
int 0x80

.done:

pop dx
pop cx
pop bx
pop ax
iret

video_initialize_screen:

push ax
push bx
push es
push fs

mov ax, 0xB800
mov es, ax
mov ax, 0x9000
mov fs, ax

xor bx, bx								; Point to video memory
mov al, byte [fs:video_char_attributes]	; Get char attributes
mov ah, byte [fs:video_cursor_attributes]	; Get cursor attributes

.draw_cursor:
mov byte [es:bx], ' '			; Draw cursor
inc bx
mov byte [es:bx], ah			; Set
inc bx

.loop:
cmp bx, 0x0FA0			; End of video memory?
je .done
mov byte [es:bx], ' '			; Draw space
inc bx
mov byte [es:bx], al			; Set
inc bx
jmp .loop

.done:
mov word [fs:video_cursor_location], 0x0000
mov byte [fs:video_cursor_enabled], 0x01

pop fs
pop es
pop bx
pop ax
iret

video_set_attributes:

push bx
push fs

mov bx, 0x9000
mov fs, bx

mov byte [fs:video_char_attributes], ah
mov byte [fs:video_cursor_attributes], al

cmp byte [fs:video_cursor_enabled], 0x01
jne .done
call video_draw_cursor

.done:

pop fs
pop bx

iret

video_disable_cursor:

push ax
push fs

mov ax, 0x9000
mov fs, ax

call video_clear_cursor
mov byte [fs:video_cursor_enabled], 0x00

pop fs
pop ax

iret

video_enable_cursor:

push ax
push fs

mov ax, 0x9000
mov fs, ax

call video_draw_cursor
mov byte [fs:video_cursor_enabled], 0x01

pop fs
pop ax

iret

video_set_cursor_position:

; IN: AH = cursor X
;     AL = cursor Y

push ax
push bx
push cx
push dx
push fs

mov bx, 0x9000
mov fs, bx

cmp byte [fs:video_cursor_enabled], 0x01
jne .main
call video_clear_cursor		; Clear cursor

.main:

push ax						; Save AX
xor ah, ah					; Clear high 8
mov bx, 160					; Multiply Y * 160
mul bx
mov word [fs:video_cursor_location], ax	; Save for now
pop ax						; Restore AX
xor al, al					; Clear low 8
shr ax, 8					; Shift right 8
add ax, ax					; Multiply X * 2
add word [fs:video_cursor_location], ax	; Add

cmp byte [fs:video_cursor_enabled], 0x01
jne .done
call video_draw_cursor		; Draw cursor back

.done:

pop fs
pop dx
pop cx
pop bx
pop ax

iret

video_get_cursor_position:

; OUT: AH = cursor X
;      AL = cursor Y

push bx
push cx
push dx
push fs

mov ax, 0x9000
mov fs, ax

mov ax, word [fs:video_cursor_location]
mov bx, 160					; Divide AX / 160
xor dx, dx
div bx
xor ah, ah					; Clear AH
push ax						; Push result, for now
mov ax, dx					; Load MOD
mov bx, 2					; Divide MOD / 2
xor dx, dx
div bx
mov dx, ax					; Result in DX
pop ax						; Restore AX
mov ah, dl					; Move AH

pop fs
pop dx
pop cx
pop bx

iret

video_clear_cursor:

push ax
push bx
push es
push fs

mov ax, 0xB800
mov es, ax
mov ax, 0x9000
mov fs, ax

mov bx, word [fs:video_cursor_location]
mov byte [es:bx], ' '
inc bx
mov al, byte [fs:video_char_attributes]
mov byte [es:bx], al

pop fs
pop es
pop bx
pop ax

ret

video_draw_cursor:

push ax
push bx
push es
push fs

mov ax, 0xB800
mov es, ax
mov ax, 0x9000
mov fs, ax

mov bx, word [fs:video_cursor_location]
mov byte [es:bx], ' '
inc bx
mov al, byte [fs:video_cursor_attributes]
mov byte [es:bx], al

pop fs
pop es
pop bx
pop ax

ret

video_new_line:

push ax

mov al, 0x0A
push 0x01
int 0x80

pop ax
iret
