fat_load_chain:

; ******************************************************************************
;     Decides whether to use the FAT12 or FAT16 routine, then passes control
; ******************************************************************************


call fat12_load_chain
ret
