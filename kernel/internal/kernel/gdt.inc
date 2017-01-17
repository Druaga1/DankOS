GDT:

dw .GDTEnd - .GDTStart - 1	; GDT size
dd GDT						; GDT start

.GDTStart:

; Null descriptor (required)

.NullDescriptor:

dw 0x0000			; Limit
dw 0x0000			; Base (low 16 bits)
db 0x00				; Base (mid 8 bits)
db 00000000b		; Access
db 00000000b		; Granularity
db 0x00				; Base (high 8 bits)

; Kernel space (full unrestricted access to all memory and ring 0)

.KernelCode:

dw 0xFFFF			; Limit
dw 0x0000			; Base (low 16 bits)
db 0x00				; Base (mid 8 bits)
db 10011010b		; Access
db 11001111b		; Granularity
db 0x00				; Base (high 8 bits)

.KernelData:

dw 0xFFFF			; Limit
dw 0x0000			; Base (low 16 bits)
db 0x00				; Base (mid 8 bits)
db 10010010b		; Access
db 11001111b		; Granularity
db 0x00				; Base (high 8 bits)

; User space (restricted access to some portions of memory, ring 3)

.UserCode:

dw 0x0000			; Limit
dw 0x0000			; Base (low 16 bits)
db 0x00				; Base (mid 8 bits)
db 00000000b		; Access
db 00000000b		; Granularity
db 0x00				; Base (high 8 bits)

.UserData:

dw 0x0000			; Limit
dw 0x0000			; Base (low 16 bits)
db 0x00				; Base (mid 8 bits)
db 00000000b		; Access
db 00000000b		; Granularity
db 0x00				; Base (high 8 bits)

.GDTEnd:
