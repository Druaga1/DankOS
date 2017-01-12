;********************************************************************************
;		
;		Simple speaker driver
;		
;********************************************************************************
speaker_beep_frequency:
		; IN	--> AX: Frequency
		
		push bx
		push cx
		push ax
        mov     al, 182         ; Prepare the speaker for the note.
        out     43h, al
		pop ax
        out     42h, al         ; Output low byte.
        mov     al, ah          ; Output high byte.
        out     42h, al 
        in      al, 61h         ; Turn on note (get value from port 61h).
   
        or      al, 00000011b   ; Set bits 1 and 0.
        out     61h, al         ; Send new value.
        mov     bx, 25          ; Pause for duration of note.
.pause1:
        mov     cx, 65535
.pause2:
        dec     cx
        jne     .pause2
        dec     bx
        jne     .pause1
        in      al, 61h         ; Turn off note (get value from port 61h).
        and     al, 11111100b   ; Reset bits 1 and 0.
        out     61h, al         ; Send new value.
		pop cx
		pop bx
		iret
;********************************************************************************