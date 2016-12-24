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
mov sp, 0xFFF0						; Move stack to 0xFFF0
sti									; Enable interrupts back

; **** Bootup routines ****

push ds								; Enable the interrupt 0x80 for the system API
xor ax, ax
mov ds, ax
mov word [0x0200], system_call
mov word [0x0202], 0x9000
pop ds

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

mov dl, byte [CurrentDrive]		; Get current drive
mov si, InitName				; Use the default 'init.bin'
mov di, ShellSwitches			; No switches
push 0x14
int 0x80						; Launch process #1

; Since process #1 is never supposed to quit, add an exception handler here

mov si, ProcessWarning1			; Print warning message (part 1)
push 0x02
int 0x80

xor dl, dl
push 0x06
int 0x80						; Print exit code

mov si, ProcessWarning2			; Print second part of message
push 0x02
int 0x80

push 0x18
int 0x80						; Pause

jmp reload						; Reload shell


start_fail:

mov si, KernelRunningMsg		; Print error and terminate execution
push 0x02
int 0x80
push 0x00
int 0x80


data:

InitName		db	'init.bin', 0x00
ProcessWarning1	db	0x0A, "Kernel: The root process has been terminated,"
				db	0x0A, "        process exit code: ", 0x00
ProcessWarning2	db	0x0A, "        The kernel will now reload 'init.bin'."
				db	0x0A, "Press a key to continue...", 0x00
KernelRunningMsg	db	"The kernel is already loaded.", 0x0A, 0x00
ShellSwitches	db	0x00


;Includes

%include 'includes/kernel/syscalls.inc'
%include 'includes/kernel/kernel.inc'
%include 'includes/kernel/video.inc'
%include 'includes/kernel/io.inc'
%include 'includes/kernel/disk.inc'
%include 'includes/kernel/timer.inc'
%include 'includes/kernel/speaker.inc'

;File systems

%include 'includes/kernel/file_systems/global.inc'
%include 'includes/kernel/file_systems/local.inc'
%include 'includes/kernel/file_systems/fat12.inc'

;ASCII splash screen

%include 'includes/kernel/splash.inc'

;Global variables

%include 'includes/kernel/variables.inc'
