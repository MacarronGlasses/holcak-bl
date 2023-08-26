; Prints out ascii symbol representing error code and halts
; Parameters:
;   al    - Error code
[bits 16]
pute:
	mov ah, 0x0E
	int 0x10
	cli
	hlt
