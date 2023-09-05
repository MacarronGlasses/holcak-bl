%define endl 0x0D, 0x0A

; Prints string ending with NUL
; Parameters:
;   ds:si - String address
; References:
;   https://stanislavs.org/helppc/int_10-e.html
[bits 16]
puts:
	push ax
	push si
	mov ah, 0x0E

.loop:
	lodsb
	test al, al
	jz .done
	int 0x10
	jmp .loop

.done:
	pop si
	pop ax
	ret

; Prints 4-bit hexadecimal value (value bigger than 0x0F results in UB)
; Parameters:
;   bl    - Value
; References:
;   https://stanislavs.org/helppc/int_10-e.html
putx4:
	push ax
	mov al, bl
	mov ah, 0x0E

	cmp al, 0x0A
	jl .dump
	add al, 0x07

.dump:
	add al, 0x30
	int 0x10

	pop ax
	ret

; Prints 8-bit hexadecimal value
; Parameters:
;   bl    - Value
putx8:
	push bx
	shr bl, 0x04
	call putx4
	mov bl, [esp]
	and bl, 0x0F
	call putx4
	pop bx
	ret

; Prints-16 bit hexadecimal value
; Parameters:
;   bx    - Value
putx16:
	push bx
	mov bl, bh
	call putx8
	pop bx
	call putx8
	ret

; Prints-32 bit hexadecimal value
; Parameters:
;   ebx   - Value
putx32:
	push ebx
	shr ebx, 0x10
	call putx16
	pop ebx
	call putx16
	ret

; Prints new line
; References:
;   https://stanislavs.org/helppc/int_10-e.html
[bits 16]
putnl:
	push ax
	mov ah, 0x0E
	mov al, 0x0A
	int 0x10
	mov al, 0x0D
	int 0x10
	pop ax
	ret
