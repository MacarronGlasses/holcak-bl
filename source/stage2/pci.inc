; Detects PCI available mechanism
; References:
;   https://wiki.osdev.org/PCI#Detecting_Configuration_Space_Access_Mechanism.2Fs
;   http://www.ctyme.com/intr/rb-2371.htm
[bits 16]
pci_init:
	pushad
	pushfd

	mov ax, 0xB101
	xor edi, edi
	int 0x1A
	test ah, ah
	jnz .done
	cmp edx, 0x20494350
	jne .done

	mov si, .msg0
	call puts
	mov dword [pci_info.entry], edi

	mov si, .msg1
	call puts
	mov word [pci_info.version], bx
	and ebx, 0xFFFF
	call putx32
	call putnl

	mov si, .msg2
	call puts
	mov byte [pci_info.hardware], al
	xor ebx, ebx
	mov bl, al
	call putx32
	call putnl

	mov si, .msg3
	call puts
	inc cx
	mov word [pci_info.buses], cx
	xor ebx, ebx
	mov bx, cx
	call putx32
	call putnl

.done:
	popfd
	popad
	ret

.msg0: db "PCI detected!", endl, 0x00
.msg1: db "    Version:   ", 0x00
.msg2: db "    Hardware:  ", 0x00
.msg3: db "    Buses:     ", 0x00

section .bss
pci_info:
	.entry:    resd 0x01
	.version:  resw 0x01
	.buses:    resw 0x01
	.hardware: resb 0x01
section .text
