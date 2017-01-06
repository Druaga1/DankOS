; *****************************************************************
;     The DankOS kernel. It contains core drivers and routines.
; *****************************************************************

org 0x0100							; Bootloader loads us here (9000:0100)
bits 16								; 16-bit Real mode

; **** Check CS using a far call to verify that we're loaded in the proper spot by the bootloader ****
; ** if not, use a 'terminate execution' interrupt to return to the caller

call 0x9000:check_cs				; Far call to the check routine

check_cs:

pop ax								; Pop call offset into AX
pop bx								; Pop call segment into BX
cmp bx, 0x9000						; Check if CS was 0x9000
jne start_fail						; If it wasn't, cleanely abort execution

cli									; Disable interrupts and set segments to 0x9000
mov ax, 0x9000
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov sp, 0x7FF0						; Move stack to 0x7FF0
sti									; Enable interrupts back

; **** Bootup routines ****

push ds								; Enable the interrupt 0x80 for the system API
xor ax, ax
mov ds, ax
mov word [0x0200], system_call
mov word [0x0202], 0x9000
pop ds

mov byte [BootDrive], dl		; Save boot drive

push 0x29				; Set current drive
int 0x80

; Prepare the screen

mov ax, 0x1003
mov bl, 0x00
xor bh, bh
int 0x10				; Disable blinking with BIOS

mov dh, 24
mov dl, 80
mov bh, 0x00
mov ah, 0x02
int 0x10				; Disable BIOS cursor

mov ah, 0x02
mov al, 0x70
push 0x11
int 0x80				; Set palette and reset screen
push 0x0A
int 0x80

mov si, SplashScreen	; Display SplashScreen
push 0x02
int 0x80

reload:

mov si, ShellName				; Use the default 'shell.bin'
mov di, ShellSwitches			; No switches
push 0x14
int 0x80						; Launch process #1

; Since process #1 is never supposed to quit, add an exception handler here

mov si, ProcessWarning1			; Print warning message (part 1)
push 0x02
int 0x80

xor cl, cl
xor dl, dl
push 0x06
int 0x80						; Print exit code

mov si, ProcessWarning2			; Print second part of message
push 0x02
int 0x80

push 0x18
int 0x80						; Pause

mov dl, byte [BootDrive]		; Set the current drive to the boot drive
push 0x29
int 0x80

jmp reload						; Reload shell


start_fail:

mov si, KernelRunningMsg		; Print error and terminate execution
push 0x02
int 0x80
push 0x00
int 0x80


data:

ShellName		db	'shell.bin', 0x00
ProcessWarning1	db	0x0A, "Kernel: The root process has been terminated,"
				db	0x0A, "        process exit code: ", 0x00
ProcessWarning2	db	0x0A, "        The kernel will now reload 'shell.bin'."
				db	0x0A, "Press a key to continue...", 0x00
KernelRunningMsg	db	"The kernel is already loaded.", 0x0A, 0x00
ShellSwitches	db	0x00
BootDrive		db	0x00

;Includes (internal routines)

%include 'includes/kernel/internal/fat_name_to_string.inc'
%include 'includes/kernel/internal/fat_time_to_integer.inc'
%include 'includes/kernel/internal/floppy_read_sector.inc'
%include 'includes/kernel/internal/floppy_write_sector.inc'
%include 'includes/kernel/internal/string_to_fat_name.inc'

;Includes (external routines)

%include 'includes/kernel/external/compare_strings.inc'
%include 'includes/kernel/external/input_integer.inc'
%include 'includes/kernel/external/input_string.inc'
%include 'includes/kernel/external/lower_to_uppercase.inc'
%include 'includes/kernel/external/pause.inc'
%include 'includes/kernel/external/print_integer.inc'
%include 'includes/kernel/external/print_integer_hex.inc'
%include 'includes/kernel/external/string_copy.inc'
%include 'includes/kernel/external/string_end.inc'
%include 'includes/kernel/external/string_length.inc'
%include 'includes/kernel/external/string_to_integer.inc'
%include 'includes/kernel/external/timer_read.inc'
%include 'includes/kernel/external/upper_to_lowercase.inc'
%include 'includes/kernel/external/floppy_read_sectors.inc'
%include 'includes/kernel/external/floppy_read_word.inc'
%include 'includes/kernel/external/floppy_read_byte.inc'
%include 'includes/kernel/external/floppy_read_dword.inc'
%include 'includes/kernel/external/floppy_write_sectors.inc'
%include 'includes/kernel/external/floppy_write_word.inc'
%include 'includes/kernel/external/floppy_write_byte.inc'
%include 'includes/kernel/external/floppy_write_dword.inc'
%include 'includes/kernel/external/enter_graphics_mode.inc'
%include 'includes/kernel/external/exit_graphics_mode.inc'
%include 'includes/kernel/external/draw_pixel.inc'
%include 'includes/kernel/external/line.inc'

;Includes (kernel routines and stuff)

%include 'includes/kernel/syscalls.inc'
%include 'includes/kernel/variables.inc'
%include 'includes/kernel/splash.inc'
%include 'includes/kernel/kernel.inc'

; TO BE SORTED

%include 'includes/kernel/video.inc'
%include 'includes/kernel/speaker.inc'

;File systems

%include 'includes/kernel/file_systems/global.inc'
%include 'includes/kernel/file_systems/local.inc'
%include 'includes/kernel/file_systems/fat12_load_chain.inc'
%include 'includes/kernel/file_systems/fat12_delete_chain.inc'
%include 'includes/kernel/file_systems/path_converter.inc'
%include 'includes/kernel/file_systems/path_resolver.inc'
