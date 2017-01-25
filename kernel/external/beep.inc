beep:

; ***********************************
;     ECX = Frequency of the beep
; ***********************************

push eax
push ebx
push ecx
push edx

; Prepare PIC

mov al, 0xB6
out 0x43, al

; Divide 1193180 / frequency

mov eax, 1193180
xor edx, edx
div ecx

; Correct value now in AX, output it

out 0x42, al		; Output low byte
mov al, ah
out 0x42, al		; Output high byte

in al, 0x61			; Connect speaker to timer 2
or al, 00000011b

out 0x61, al

pop edx
pop ecx
pop ebx
pop eax
iret
