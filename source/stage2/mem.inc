; Detects free regions of memory
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
[bits 16]
mem_init:
	ret

; Detects free regions of memory with interrupt 0xE820
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
[bits 16]
mem_init_0xE820:
	ret

mem_data_size: equ 0xFF
section .bss
struc mem_node
	.next:  resd 0x01
	.base:  resq 0x01
	.limit: resq 0x01
	.type:  resd 0x01
	.acpi:  resd 0x01
endstruc
mem_info:
	.node:  resd 0x01
	.bytes: resb 0x01
mem_data:   resb mem_data_size*mem_node_size
section .text
