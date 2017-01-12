put_char:

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

mov bx, KernelSpace							; Point FS to kernel space
mov fs, bx

cmp al, 0x0A					; Is 0x0A?
je .next_line

cmp al, 0x08					; Is 0x08?
je .backspace

cmp al, 0x09					; Is 0x09?
je .tab

mov bx, word [fs:CursorLocation]
inc bx
mov ah, byte [fs:CharAttributes]
mov byte [es:bx], ah						; Attributes first to avoid 'ghosting'
dec bx
mov byte [es:bx], al							; Print character
inc bx
inc bx
cmp bx, 0x0FA0						; End of video memory?
je .scroll_screen_down
mov word [fs:CursorLocation], bx
cmp byte [fs:CursorStatus], 1	; Check if cursor is enabled
jne .done
call draw_cursor				; Draw cursor
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
cmp byte [fs:CursorStatus], 1
jne .back_no_cursor
call clear_cursor
.back_no_cursor:
sub word [fs:CursorLocation], 2
cmp byte [fs:CursorStatus], 1
jne .done
call draw_cursor
jmp .done

.scroll_screen_down:
call clear_cursor			; Destroy cursor
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
mov word [fs:CursorLocation], bx	; New cursor location
mov al, byte [fs:CharAttributes]

.clear_line_loop:

cmp bx, 0x0FA0
je .clear_line_done
mov byte [es:bx], ' '
inc bx
mov byte [es:bx], al
inc bx
jmp .clear_line_loop

.clear_line_done:

cmp byte [fs:CursorStatus], 1
jne .done
call draw_cursor			; Restore cursor

.done:
pop fs
pop es
pop dx
pop cx
pop bx
pop ax
iret

.tab_loop_flag					db 0x00
