; __cdecl extern pci_info_t *pci_init(void);
[extern pci_init]
pci_init:
	push ebp
	mov ebp, esp

	xor eax, eax
	not eax

	mov esp, ebp
	pop ebp
	ret

section .bss
pci_info:
	.version:   dw 0x00
	.mechanism: db 0x00
	.buses:     db 0x00
section .text
