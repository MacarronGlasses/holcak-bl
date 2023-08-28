; Enables A20 line
; References:
;   https://wiki.osdev.org/A20_Line#Enabling
[bits 16]
a20_enable:
	push ax
	call a20_test
	jc .done

.try_0:
	mov ax, 0x2403
	int 0x15
	call a20_test
	jc .done

.try_1:
	xor al, al
	mov ah, 0x20
	in al, 0x92
	or al, 2
	out 0x92, al
	call a20_test
	jc .done
	test ah, ah
	dec ah
	jnz .try_1

.try_2:
	in al, 0xEE
	call a20_test
	jc .done

.error:
	mov si, .msg1
	call puts
	cli
	hlt

.done:
	push si
	mov si, .msg0
	call puts
	pop si

	pop ax
	ret

.msg0: db "A20 enabled!", endl, 0x00
.msg1: db "Error: Enabling A20 failed!", endl, 0x00

; Tests if A20 line is enabled
; Returns:
;   cf    - 1 if enabled
; References:
;   https://wiki.osdev.org/A20_Line#Testing_the_A20_line
[bits 16]
a20_test:
	push ax
	push bx
	push ds
	push es
	clc

	; Initialize registers
	xor ax, ax
	mov ds, ax
	mov si, 0x500

	not ax
	mov es, ax
	mov di, 0x0510

	; Save old values
	mov bl, [ds:si]
	mov bh, [es:di]

	; Test if enabled
	mov ah, 0x01
	mov [ds:si], byte 0x00
	mov [es:di], byte 0x01
	mov al, [ds:si]
	cmp al, [es:di]
	je .done
	stc

.done:
	; Restore values
	mov [ds:si], bl
	mov [es:di], bh

	pop es
	pop ds
	pop bx
	pop ax
	ret
