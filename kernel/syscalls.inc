system_call:

;		** int 0x80 has been called **

; ** check for break flag **

push ax
push gs

mov ax, KernelSpace
mov gs, ax

cmp byte [gs:BreakFlag], 0x01

pop gs
pop ax

je .break_execution

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
je put_char

xchg sp, bp
cmp word [ss:(bp-2)], 0x02
xchg sp, bp
je print_string

xchg sp, bp
cmp word [ss:(bp-2)], 0x03
xchg sp, bp
je new_line

xchg sp, bp
cmp word [ss:(bp-2)], 0x04
xchg sp, bp
je string_to_integer

xchg sp, bp
cmp word [ss:(bp-2)], 0x05
xchg sp, bp
je print_integer_hex

xchg sp, bp
cmp word [ss:(bp-2)], 0x06
xchg sp, bp
je print_integer

xchg sp, bp
cmp word [ss:(bp-2)], 0x07
xchg sp, bp
je input_integer

xchg sp, bp
cmp word [ss:(bp-2)], 0x08
xchg sp, bp
je compare_strings

xchg sp, bp
cmp word [ss:(bp-2)], 0x09
xchg sp, bp
je string_length

xchg sp, bp
cmp word [ss:(bp-2)], 0x0A
xchg sp, bp
je initialise_screen

xchg sp, bp
cmp word [ss:(bp-2)], 0x0B
xchg sp, bp
je disable_cursor

xchg sp, bp
cmp word [ss:(bp-2)], 0x0C
xchg sp, bp
je enable_cursor

xchg sp, bp
cmp word [ss:(bp-2)], 0x0D
xchg sp, bp
je get_cursor_position

xchg sp, bp
cmp word [ss:(bp-2)], 0x0E
xchg sp, bp
je set_cursor_position

xchg sp, bp
cmp word [ss:(bp-2)], 0x0F
xchg sp, bp
je center_print_string

xchg sp, bp
cmp word [ss:(bp-2)], 0x10
xchg sp, bp
je input_string

xchg sp, bp
cmp word [ss:(bp-2)], 0x11
xchg sp, bp
je set_palette

xchg sp, bp
cmp word [ss:(bp-2)], 0x12
xchg sp, bp
je load_file

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
je lower_to_uppercase

xchg sp, bp
cmp word [ss:(bp-2)], 0x16
xchg sp, bp
je upper_to_lowercase

xchg sp, bp
cmp word [ss:(bp-2)], 0x17
xchg sp, bp
je get_current_palette

xchg sp, bp
cmp word [ss:(bp-2)], 0x18
xchg sp, bp
je pause

xchg sp, bp
cmp word [ss:(bp-2)], 0x19
xchg sp, bp
je allocate_memory

xchg sp, bp
cmp word [ss:(bp-2)], 0x1A
xchg sp, bp
je cut_string

xchg sp, bp
cmp word [ss:(bp-2)], 0x1B
xchg sp, bp
je ascii_dump

xchg sp, bp
cmp word [ss:(bp-2)], 0x1C
xchg sp, bp
je get_char

xchg sp, bp
cmp word [ss:(bp-2)], 0x1D
xchg sp, bp
je sleep

xchg sp, bp
cmp word [ss:(bp-2)], 0x1E
xchg sp, bp
je stop_beep

xchg sp, bp
cmp word [ss:(bp-2)], 0x1F
xchg sp, bp
je play_music






xchg sp, bp
cmp word [ss:(bp-2)], 0x20
xchg sp, bp
je timer_read

xchg sp, bp
cmp word [ss:(bp-2)], 0x21
xchg sp, bp
je load_dir

xchg sp, bp
cmp word [ss:(bp-2)], 0x22
xchg sp, bp
je beep

xchg sp, bp
cmp word [ss:(bp-2)], 0x23
xchg sp, bp
je floppy_read_sectors

xchg sp, bp
cmp word [ss:(bp-2)], 0x24
xchg sp, bp
je floppy_read_byte

xchg sp, bp
cmp word [ss:(bp-2)], 0x25
xchg sp, bp
je floppy_read_word

xchg sp, bp
cmp word [ss:(bp-2)], 0x26
xchg sp, bp
je floppy_read_dword

xchg sp, bp
cmp word [ss:(bp-2)], 0x27
xchg sp, bp
je string_copy

xchg sp, bp
cmp word [ss:(bp-2)], 0x28
xchg sp, bp
je directory_scanner

xchg sp, bp
cmp word [ss:(bp-2)], 0x29
xchg sp, bp
je set_current_drive






xchg sp, bp
cmp word [ss:(bp-2)], 0x2B
xchg sp, bp
je ping_file






xchg sp, bp
cmp word [ss:(bp-2)], 0x2C
xchg sp, bp
je fat_time_to_integer

xchg sp, bp
cmp word [ss:(bp-2)], 0x2D
xchg sp, bp
je string_end

xchg sp, bp
cmp word [ss:(bp-2)], 0x2E
xchg sp, bp
je get_current_dir









xchg sp, bp
cmp word [ss:(bp-2)], 0x31
xchg sp, bp
je floppy_write_sectors

xchg sp, bp
cmp word [ss:(bp-2)], 0x32
xchg sp, bp
je floppy_write_byte

xchg sp, bp
cmp word [ss:(bp-2)], 0x33
xchg sp, bp
je floppy_write_word

xchg sp, bp
cmp word [ss:(bp-2)], 0x34
xchg sp, bp
je floppy_write_dword

xchg sp, bp
cmp word [ss:(bp-2)], 0x35
xchg sp, bp
je invalid_cache









xchg sp, bp
cmp word [ss:(bp-2)], 0x80
xchg sp, bp
je enter_graphics_mode

xchg sp, bp
cmp word [ss:(bp-2)], 0x81
xchg sp, bp
je draw_pixel

xchg sp, bp
cmp word [ss:(bp-2)], 0x82
xchg sp, bp
je exit_graphics_mode

xchg sp, bp
cmp word [ss:(bp-2)], 0x83
xchg sp, bp
je draw_line

xchg sp, bp
cmp word [ss:(bp-2)], 0x84
xchg sp, bp
je draw_sprite

xchg sp, bp
cmp word [ss:(bp-2)], 0x85
xchg sp, bp
je clear_screen

xchg sp, bp
cmp word [ss:(bp-2)], 0x86
xchg sp, bp
je push_frame

xchg sp, bp
cmp word [ss:(bp-2)], 0x87
xchg sp, bp
je get_version_number



; 32-bit calls

xchg sp, bp
cmp word [ss:(bp-2)], 0xA0
xchg sp, bp
je allocate_mem32








; *** Invalid system call exception handler

mov ax, KernelSpace
mov ds, ax
mov si, .invalid_call_msg
push 0x02
int 0x80
push 0x00
int 0x80


.invalid_call_msg		db	0x0A, "Kernel: An invalid system call has been issued by the program."
						db	0x0A, "        Execution aborted.", 0x0A, 0x00

.break_execution:

mov ax, KernelSpace
mov ds, ax
mov byte [BreakFlag], 0x00
mov eax, 0xFFFFFFFE
push 0x00
int 0x80
