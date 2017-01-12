fat_delete_chain:

; ******************************************************************************
;     Decides whether to use the FAT12 or FAT16 routine, then passes control
; ******************************************************************************


call fat12_delete_chain
ret
