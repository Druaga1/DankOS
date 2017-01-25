; **********************************************************
;     This file contains all the global Kernel variables
; **********************************************************

; VERSION NUMBER - CHANGE THIS EACH RELEASE

VersionNumber:
.Alpha	db	11
.Beta	db	0
.Minor	db	0
.Major	db	0

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

KernelSpace				equ	0xFFFF
TopSegment				dw	0x1050
TopMemory				dd	0x00100000
StackPointer			dd	0x00000000
StackSegment			dw	0x0000
FrameBuffer				equ	0x0050
ClockTicks				dd	0x00000000
BreakFlag				db	0x00

; Music

MusicPlayingFlag		db	0x00
MusicRepeatFlag			db	0x00
MusicStartAddress		dd	0x00000000
MusicCurrentNote		dd	0x00000000
NoteLengthCounter		db	0x00
MusicSegment			dw	0x0000

; Disk

CacheStatus				db	0x00
DriveInCache			db	0x00
SectorInCache			dw	0x0000
DiskCache				times 512 db 0x00
