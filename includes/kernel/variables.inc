; **********************************************************
;     This file contains all the global Kernel variables
; **********************************************************

; File system

CurrentDrive			db	0x00
CurrentDirectoryCache	equ 0xC000
CurrentDirectoryPath	times 129 db 0x00
CurrentDirectoryCluster	dw	0x0000

; Kernel

KernelSpace				equ	0xFFFF
TopSegment				dw	0x1050
StackPointer			dd	0x00000000
StackSegment			dw	0x0000
FrameBuffer				equ	0x0050

; Disk

CacheStatus				db	0x00
DriveInCache			db	0x00
SectorInCache			dw	0x0000
DiskCache				times 512 db 0x00
