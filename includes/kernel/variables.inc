; **********************************************************
;     This file contains all the global Kernel variables
; **********************************************************

CurrentDrive			db	0x00		; The default drive of the kernel
TopSegment				dw	0x0050		; The bottom of the free space (segment), when the kernel is started is 0050:0000
StackPointer			dd	0x00000000	; Stack pointer
StackSegment			dw	0x0000		; Stack segment
