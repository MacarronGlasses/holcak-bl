section .text
[extern main]

[bits 16]
%include "puts.inc"
%include "cpuid.inc"
%include "mem.inc"
%include "pci.inc"
%include "a20.inc"

; TODO: Add keyboard controller method to A20 line
; TODO: Add more memory detection methods (https://wiki.osdev.org/Detecting_Memory_(x86)#Other_Methods)
; TODO: Enable extended CPU features

section .entry
[global start_16]

[bits 16]
start_16:
	; Initialize registers
	cli
	mov	sp, 0x1000
	mov bp, sp

	; Gather information from BIOS
	call cpuid_init
	call pci_init
	call mem_init
	call a20_enable

	; Enter protected mode
	lgdt [gdt_info.desc]
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax

	jmp code_seg:start_32

[bits 32]
start_32:
	; Initialize registers
	cli
	mov ebp, stack_data + stack_size
	mov esp, ebp

	; Jump to stage3
	call main
	cli
	hlt

[bits 32]
%include "gdt.inc"

stack_size: equ 0x2000
section .bss
stack_data:
	resd stack_size
section .text
