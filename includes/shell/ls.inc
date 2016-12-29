; **** ls command ****

ls_cmd:

mov di, .FileName

mov word [.EntryCounter], 0x0000

.loop:

mov ax, word [.EntryCounter]

push 0x28
int 0x80

jc prompt_loop			; If end of table, return to prompt

inc word [.EntryCounter]

mov dword [.Size], ecx	; Save size
mov byte [.Directory], dl

push 0x2C				; Convert to standard time
int 0x80

mov byte [.Seconds], al
mov byte [.Minutes], ah
mov byte [.Hours], bl
mov byte [.Days], bh
mov byte [.Months], cl
mov word [.Years], dx

cmp byte [.Directory], 0xFF		; Check if the entry is a directory
jne .skip_bracket1

mov al, '['
push 0x01
int 0x80
.skip_bracket1:

mov si, .FileName		; Print file name
push 0x02
int 0x80

cmp byte [.Directory], 0xFF		; Check if the entry is a directory
jne .skip_bracket2

mov al, ']'
push 0x01
int 0x80
.skip_bracket2:

push 0x0D
int 0x80				; Get cursor coordinates

mov ah, 15				; Set the X to 15

push 0x0E
int 0x80				; Set cursor coordinates

cmp byte [.Directory], 0xFF		; Check if the entry is a directory
je .skip_size

xor cl, cl
mov eax, dword [.Size]	; Print size, right align
mov dl, 0x01
push 0x06
int 0x80

mov si, .bytes_msg		; Print bytes
push 0x02
int 0x80
.skip_size:

push 0x0D
int 0x80				; Get cursor coordinates

mov ah, 35				; X = 35

push 0x0E
int 0x80				; Set cursor coordinates

xor dl, dl				; Left align

mov cl, 2
xor eax, eax			; Print days
mov al, byte [.Days]
push 0x06
int 0x80

mov al, '/'				; Print separator
push 0x01
int 0x80

xor eax, eax			; Print months
mov al, byte [.Months]
push 0x06
int 0x80

mov al, '/'				; Print separator
push 0x01
int 0x80

mov cl, 4
xor eax, eax			; Print years
mov ax, word [.Years]
push 0x06
int 0x80

push 0x0D
int 0x80				; Get cursor coordinates

add ah, 1				; X = X + 1

push 0x0E
int 0x80				; Set cursor coordinates

mov cl, 2
xor eax, eax			; Print hours
mov al, byte [.Hours]
push 0x06
int 0x80

mov al, ':'				; Print separator
push 0x01
int 0x80

xor eax, eax			; Print minutes
mov al, byte [.Minutes]
push 0x06
int 0x80

mov al, ':'				; Print separator
push 0x01
int 0x80

xor eax, eax			; Print seconds
mov al, byte [.Seconds]
push 0x06
int 0x80

push 0x03				; Next line
int 0x80

jmp .loop


.Directory		db	0x00
.bytes_msg		db	' bytes', 0x00
.EntryCounter	dw	0x0000
.Seconds		db	0x00
.Minutes		db	0x00
.Hours			db	0x00
.Days			db	0x00
.Months			db	0x00
.Years			dw	0x0000
.Size			dd	0x00000000
.FileName		times 13 db 0x00











