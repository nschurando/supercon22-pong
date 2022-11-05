	; Move led matrix over to page 2
	mov r0, 2
	mov [0xF0], r0
	
	; Send letter A repeatedly
	mov r2, 0b0100               ; High nibble of ASCII "A"
	mov r3, 0b0001               ; Low nibble of ASCII "A"
	mov r0, r2                   ; Write high nibble to UART
	mov [0xF7], r0               ;
	mov r0, r3                   ; Write low nibble to UART
	mov [0xF6], r0               ; Transmit
