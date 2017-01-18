IDT:

dw .IDTEnd - .IDTStart - 1	; IDT size
dd .IDTStart				; IDT start

.IDTStart:

dq 0

.IDTEnd:
