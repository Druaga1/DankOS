CPUStatus:

.ESP		dd	0x00007FF0
.DS			dw	0x0010
.ES			dw	0x0010
.FS			dw	0x0010
.GS			dw	0x0010
.SS			dw	0x0010

.RealModeStack		times 128 db 0x00
.ReservedStack		dd 0x00000000
