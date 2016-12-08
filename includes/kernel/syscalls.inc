system_call:

;		** int 0x80 has been called **

; CURRENT STACK: function number|flags|return offset|return segment

push bp

; CURRENT STACK: function number|flags|return offset|return segment|bp

mov bp, sp
add bp, 8
push ax

; CURRENT STACK: function number|flags|return offset|return segment|bp|ax

push bx

; CURRENT STACK: function number|flags|return offset|return segment|bp|ax|bx

mov ax, word [ss:bp]
sub bp, 2
mov bx, word [ss:bp]
add bp, 2
mov word [ss:bp], bx

; CURRENT STACK: flags|flags|return offset|return segment|bp|ax|bx

sub bp, 4
mov bx, word [ss:bp]
add bp, 2
mov word [ss:bp], bx

; CURRENT STACK: flags|return offset|return offset|return segment|bp|ax|bx

sub bp, 4
mov bx, word [ss:bp]
add bp, 2
mov word [ss:bp], bx

; CURRENT STACK: flags|return offset|return segment|return segment|bp|ax|bx

sub bp, 2
mov word [ss:bp], ax

; CURRENT STACK: flags|return offset|return segment|function number|bp|ax|bx

pop bx
pop ax
pop bp

; TARGET STACK: flags|return offset|return segment|function number

add sp, 2

;	** Function list **

xchg sp, bp
cmp word [ss:(bp-2)], 0x00
xchg sp, bp
je terminate_process

xchg sp, bp
cmp word [ss:(bp-2)], 0x01
xchg sp, bp
je video_put_char

xchg sp, bp
cmp word [ss:(bp-2)], 0x02
xchg sp, bp
je video_print_string

xchg sp, bp
cmp word [ss:(bp-2)], 0x03
xchg sp, bp
je video_new_line

xchg sp, bp
cmp word [ss:(bp-2)], 0x04
xchg sp, bp
je function_string_to_integer

xchg sp, bp
cmp word [ss:(bp-2)], 0x06
xchg sp, bp
je function_print_integer

xchg sp, bp
cmp word [ss:(bp-2)], 0x07
xchg sp, bp
je function_input_integer

xchg sp, bp
cmp word [ss:(bp-2)], 0x08
xchg sp, bp
je function_compare_strings

xchg sp, bp
cmp word [ss:(bp-2)], 0x09
xchg sp, bp
je function_string_length

xchg sp, bp
cmp word [ss:(bp-2)], 0x0A
xchg sp, bp
je video_initialize_screen

xchg sp, bp
cmp word [ss:(bp-2)], 0x0B
xchg sp, bp
je video_disable_cursor

xchg sp, bp
cmp word [ss:(bp-2)], 0x0C
xchg sp, bp
je video_enable_cursor

xchg sp, bp
cmp word [ss:(bp-2)], 0x0D
xchg sp, bp
je video_get_cursor_position

xchg sp, bp
cmp word [ss:(bp-2)], 0x0E
xchg sp, bp
je video_set_cursor_position

xchg sp, bp
cmp word [ss:(bp-2)], 0x0F
xchg sp, bp
je video_center_print_string

xchg sp, bp
cmp word [ss:(bp-2)], 0x10
xchg sp, bp
je function_input_string

xchg sp, bp
cmp word [ss:(bp-2)], 0x11
xchg sp, bp
je video_set_attributes

xchg sp, bp
cmp word [ss:(bp-2)], 0x12
xchg sp, bp
je fat12_load_file

xchg sp, bp
cmp word [ss:(bp-2)], 0x13
xchg sp, bp
je get_current_drive

xchg sp, bp
cmp word [ss:(bp-2)], 0x14
xchg sp, bp
je start_new_program

xchg sp, bp
cmp word [ss:(bp-2)], 0x15
xchg sp, bp
je function_lower_to_uppercase

xchg sp, bp
cmp word [ss:(bp-2)], 0x16
xchg sp, bp
je function_upper_to_lowercase

xchg sp, bp
cmp word [ss:(bp-2)], 0x17
xchg sp, bp
je function_string_to_fat_name

xchg sp, bp
cmp word [ss:(bp-2)], 0x18
xchg sp, bp
je function_pause

xchg sp, bp
cmp word [ss:(bp-2)], 0x19
xchg sp, bp
je allocate_memory

jmp $
