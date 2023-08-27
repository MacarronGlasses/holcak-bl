section .entry
[global entry_16]
[extern start_16]

; TODO: Memset .bss section to zero

[bits 16]
entry_16:
	; Initialize registers
	cli
	mov ax, ds
	mov ss, ax
	mov sp, 0x1000
	mov bp, sp

	; Jump to main function
	xor dh, dh
	push dx
	push dx
	call start_16

	cli
	hlt
