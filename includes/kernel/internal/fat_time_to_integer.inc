fat_time_to_integer:

;   Transforms raw FAT time into usable integers
;     AX = FAT time, BX = FAT date
;     Returns:
;     AL = Seconds
;     AH = Minutes
;     BL = Hours
;     BH = Days
;     CL = Months
;     DX = Years

push ds

mov dx, KernelSpace
mov ds, dx

mov word [.RawTime], ax
mov word [.RawDate], bx

; Convert seconds

mov ax, word [.RawTime]

and ax, 0000000000011111b		; Extract seconds/2
mov bx, 2						; Seconds*2
mul bx

mov byte [.Seconds], al			; Save

; Convert minutes

mov ax, word [.RawTime]

and ax, 0000011111100000b		; Extract minutes
shr ax, 5						; Adjust it

mov byte [.Minutes], al			; Save

; Convert hours

mov ax, word [.RawTime]

and ax, 1111100000000000b		; Extract hours
shr ax, 11						; Adjust it

mov byte [.Hours], al			; Save

; Convert day

mov ax, word [.RawDate]

and ax, 0000000000011111b		; Extract day

mov byte [.Day], al				; Save

; Convert month

mov ax, word [.RawDate]

and ax, 0000000111100000b		; Extract month
shr ax, 5						; Adjust it

mov byte [.Month], al			; Save

; Convert year

mov ax, word [.RawDate]

and ax, 1111111000000000b		; Extract year-1980
shr ax, 9						; Adjust it
add ax, 1980					; Add 1980

mov word [.Year], ax			; Save

; *** Copy results in the proper registers ***

mov al, byte [.Seconds]
mov ah, byte [.Minutes]
mov bl, byte [.Hours]
mov bh, byte [.Day]
mov cl, byte [.Month]
mov dx, word [.Year]

pop ds
iret

.RawDate	dw 0x0000
.RawTime	dw 0x0000
.Seconds	db 0x00
.Minutes	db 0x00
.Hours		db 0x00
.Day		db 0x00
.Month		db 0x00
.Year		dw 0x0000
