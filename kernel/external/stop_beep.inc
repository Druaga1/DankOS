stop_beep:

push ax

in al, 0x61			; Disconnect speaker from timer 2
and al, 11111100b

out 0x61, al

pop ax
iret
