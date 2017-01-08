; ******************************************************
;     STAGE2:   Loads KERNEL.BIN in high memory area
; ******************************************************

org 0x0500						; We are at 0000:0500
bits 16							; in real mode

; ** Segments and stack were already set up properly by the bootloader for 0x0000 / 0x0000:0xFFF0
; ** In this case there is no need to set them up again
; ** It also passes the boot drive in DL

mov si, stage2_msg				; Print loading message using simple print (BIOS)
call simple_print

mov si, a20_msg					; Show A20 message
call simple_print

call enable_a20					; Enable the A20 address line to access high memory
jc a20_err						; If it fails, print an error and halt

mov si, kernel_msg				; Show loading kernel message
call simple_print

xor ax, ax						; Set destination segment for kernel (ES) to 0xFFFF
not ax
mov es, ax
mov si, kernel_name				; Kernel FAT12 file name
mov bx, 0x0100					; Load at ES:BX (0xFFFF:0x0100)
call fat12_load_file

jc kernel_error					; If loading fails, show kernel error message and halt

jmp 0xFFFF:0x0100				; Jump to kernel

a20_err:
mov si, a20_err_msg
call simple_print
jmp halt

kernel_error:
mov si, kernel_err_msg
call simple_print
jmp halt

halt:							; Halt system without wasting resources
hlt
jmp halt

stage2_msg		db 0x0D, 0x0A, 'Loading DankOS...', 0x0D, 0x0A, 0x00
a20_msg			db 0x0D, 0x0A, 'Enabling A20 line...', 0x00
kernel_msg		db '  DONE', 0x0D, 0x0A, 'Loading kernel...', 0x00
a20_err_msg		db 0x0D, 0x0A, 0x0A, 'An error occurred while enabling the A20 line, system halted.', 0x00
kernel_err_msg	db 0x0D, 0x0A, 0x0A, 'An error occurred while loading the Kernel, system halted.', 0x00

kernel_name		db 'KERNEL  SYS'

%include 'bootloader/bootloader_functions/simple_print.inc'
%include 'bootloader/bootloader_functions/boot_disk_functions.inc'
%include 'bootloader/bootloader_functions/boot_fat12_functions.inc'
%include 'bootloader/bootloader_functions/a20_enabler.inc'
