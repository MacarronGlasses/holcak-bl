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

; Prints 32-bit hexadecimal number
; Parameters:
;   ebx   - Hexadecimal number
; References:
;   https://stanislavs.org/helppc/int_10-e.html
[bits 16]
put32:
	push eax
	push ebx
	push cx

	mov ah, 0x0E
	mov cx, 0x08

.loop:
	mov eax, ebx
	shr eax, 0x1C

	cmp al, 0x0A
	jl .dump
	add al, 0x07

.dump:
	add al, 0x30
	mov ah, 0x0E
	int 0x10

	shl ebx, 0x04
	loop .loop

.done:
	pop cx
	pop ebx
	pop eax
	ret

; Prints 32-bit hexadecimal number with prefix
; Parameters:
;   ebx   - Hexadecimal number
putx32:
	push ax
	mov ah, 0x0E
	mov al, 0x30
	int 0x10
	mov al, 0x78
	int 0x10
	call put32
	pop ax
	ret

; Prints 64-bit hexadecimal number with prefix
; Parameters:
;   ebx   - Low part of hexadecimal number
;   ecx   - High part of hexadecimal number
putx64:
	push ebx
	mov ebx, ecx
	call putx32
	pop ebx
	call put32
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
