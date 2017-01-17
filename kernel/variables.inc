; **********************************************************
;     This file contains all the global Kernel variables
; **********************************************************

; VERSION NUMBER - CHANGE THIS EACH RELEASE

VersionNumber			dd	0x00000009

; File system

CurrentDrive			db	0x00
CurrentDirectoryCache	equ 0xC000
CurrentDirectoryPath	times 129 db 0x00
CurrentDirectoryCluster	dw	0x0000

; Video

CursorLocation			dw 0x0000
CharAttributes			db 0x07
CursorAttributes		db 0x70
CursorStatus			db 0x01

; Kernel

KernelSpace				equ	0x0000
TopSegment				dw	0x2000
StackPointer			dd	0x00000000
StackSegment			dw	0x0000
FrameBuffer				equ	0x1000

; Disk

CacheStatus				db	0x00
DriveInCache			db	0x00
SectorInCache			dw	0x0000
DiskCache				times 512 db 0x00
