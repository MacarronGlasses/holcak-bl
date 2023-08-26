section .entry

[bits 16]
[extern start_16]
start_16:
	mov al, '0'
	mov ah, 0x0E
	int 0x10

	cli
	hlt
