; Initialise system call interrupts for the kernel

push es					; Point ES to segment 0x0000 to reach the Interrupt Vector Table
xor ax, ax
mov es, ax
mov ax, 0x9000

mov word [es:0x0100], terminate_process				; int 0x40  --  terminate process
mov word [es:0x0102], ax

mov word [es:0x0104], video_put_char				; int 0x41  --  print character
mov word [es:0x0106], ax

mov word [es:0x0108], video_print_string			; int 0x42  --  print string
mov word [es:0x010A], ax

mov word [es:0x010C], video_new_line				; int 0x43  --  new line
mov word [es:0x010E], ax

mov word [es:0x0110], function_print_integer16		; int 0x44  --  print a 16-bit integer
mov word [es:0x0112], ax

mov word [es:0x0114], function_input_integer16		; int 0x45  --  input a 16-bit integer
mov word [es:0x0116], ax

mov word [es:0x0118], function_print_integer32		; int 0x46  --  print a 32-bit integer
mov word [es:0x011A], ax

mov word [es:0x011C], function_input_integer32		; int 0x47  --  input a 32-bit integer
mov word [es:0x011E], ax

mov word [es:0x0120], function_compare_strings		; int 0x48  --  compare strings
mov word [es:0x0122], ax

mov word [es:0x0124], function_string_length		; int 0x49  --  get string length
mov word [es:0x0126], ax

mov word [es:0x0128], video_initialize_screen		; int 0x4A  --  reinitialise screen
mov word [es:0x012A], ax

mov word [es:0x012C], video_disable_cursor			; int 0x4B  --  disable cursor
mov word [es:0x012E], ax

mov word [es:0x0130], video_enable_cursor			; int 0x4C  --  enable cursor
mov word [es:0x0132], ax

mov word [es:0x0134], video_get_cursor_position		; int 0x4D  --  get cursor position
mov word [es:0x0136], ax

mov word [es:0x0138], video_set_cursor_position		; int 0x4E  --  set cursor position
mov word [es:0x013A], ax

mov word [es:0x013C], video_center_print_string		; int 0x4F  --  print centered string
mov word [es:0x013E], ax

mov word [es:0x0140], function_input_string			; int 0x50  --  input string from keyboard
mov word [es:0x0142], ax

mov word [es:0x0144], video_set_attributes			; int 0x51  --  set video attributes
mov word [es:0x0146], ax

mov word [es:0x0148], fat12_load_file				; int 0x52  --  load file
mov word [es:0x014A], ax

mov word [es:0x014C], get_current_drive				; int 0x53  --  get current drive
mov word [es:0x014E], ax

mov word [es:0x0150], start_new_program				; int 0x54  --  start a new process
mov word [es:0x0152], ax

mov word [es:0x0154], function_lower_to_uppercase	; int 0x55  --  lower to uppercase
mov word [es:0x0156], ax

mov word [es:0x0158], function_upper_to_lowercase	; int 0x56  --  upper to lowercase
mov word [es:0x015A], ax

mov word [es:0x015C], function_string_to_fat_name	; int 0x57  --  convert to fat name
mov word [es:0x015E], ax

mov word [es:0x0160], function_pause				; int 0x58  --  pause
mov word [es:0x0162], ax

mov word [es:0x0164], allocate_memory				; int 0x59  --  allocate memory
mov word [es:0x0166], ax

pop es
