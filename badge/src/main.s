; 1:	| X     X|
; 2:	|  X   X |
; 3:	|   XXX  |
; 4:	| XXXXXXX|
; 5:	|        |
; 6:	| X     X|
; 7:	| X     X|
; 8:	|  XXXXX |
; 9:	|        |
; 10:	|  XXXXXX|
; 11:	| XX x   |
; 12:	|  XXXXXX|
; 13:	|        |
; 14:	| XXXXXXX|
; 15:	|    X   |
; 16:	| XXXXXXX|
; Set LED matrix memory page
mov r0,2
mov [0xf0],r0
; Set CPU clock speed
mov r0,9
mov [0xf1],r0
; Begin graphics data
mov r0,0b0100
mov [48],r0
mov r0,0b0001
mov [32],r0
mov r0,0b0010
mov [49],r0
mov r0,0b0010
mov [33],r0
mov r0,0b0001
mov [50],r0
mov r0,0b1100
mov [34],r0
mov r0,0b0111
mov [51],r0
mov r0,0b1111
mov [35],r0
mov r0,0b0000
mov [52],r0
mov r0,0b0000
mov [36],r0
mov r0,0b0100
mov [53],r0
mov r0,0b0001
mov [37],r0
mov r0,0b0100
mov [54],r0
mov r0,0b0001
mov [38],r0
mov r0,0b0011
mov [55],r0
mov r0,0b1110
mov [39],r0
mov r0,0b0000
mov [56],r0
mov r0,0b0000
mov [40],r0
mov r0,0b0011
mov [57],r0
mov r0,0b1111
mov [41],r0
mov r0,0b0110
mov [58],r0
mov r0,0b1000
mov [42],r0
mov r0,0b0011
mov [59],r0
mov r0,0b1111
mov [43],r0
mov r0,0b0000
mov [60],r0
mov r0,0b0000
mov [44],r0
mov r0,0b0111
mov [61],r0
mov r0,0b1111
mov [45],r0
mov r0,0b0000
mov [62],r0
mov r0,0b1000
mov [46],r0
mov r0,0b0111
mov [63],r0
mov r0,0b1111
mov [47],r0
