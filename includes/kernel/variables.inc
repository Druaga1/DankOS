; **********************************************************
;     This file contains all the global Kernel variables
; **********************************************************

; File system

CurrentDrive			db	0x00
DirectoryWorkingCache	equ	0x8000
CurrentDirectoryCache	equ 0xC000
CurrentDirectoryPath	times 129 db 0x00

; Kernel

TopSegment				dw	0x0050
StackPointer			dd	0x00000000
StackSegment			dw	0x0000

; Disk

CacheStatus				db	0x00
DriveInCache			db	0x00
SectorInCache			dw	0x0000
DiskCache				times 512 db 0x00
