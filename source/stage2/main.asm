section .text
[extern main]

%include "puts.inc"
%include "cpuid.inc"
%include "mem.inc"
%include "pci.inc"
%include "a20.inc"
%include "gdt.inc"

; TODO: Add keyboard controller method to A20 line
; TODO: Initialize MSRs

section .entry
[global start_16]

[bits 16]
start_16:
	; Initialize registers
	cli
	mov ax, ds
	mov ss, ax
	mov sp, 0x1000
	mov bp, sp

	; Gather information from BIOS
	call cpuid_init
	call pci_init
	call a20_enable
	call mem_init

	mov ebx, main
	call putx

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
;	call main
	cli
	hlt

stack_size: equ 0x2000
section .bss
stack_data:
	resd stack_size
section .text
