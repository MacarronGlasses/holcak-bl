; Upgraded version of cpuid instruction
; Parameters:
;   %1    - Level
%macro cpuid 1
	mov eax, %1
	cpuid
%endmacro

; Initializes CPUID instruction and MSRs
; References:
;   https://www.felixcloutier.com/x86/cpuid
;   https://en.wikipedia.org/wiki/CPUID
[bits 16]
cpuid_init:
	pushad
	pushfd

	; Check if CPUID exist
	call cpuid_exist
	jz .done
	mov si, .msg0
	call puts

	; Get CPU vendor id
	cpuid 0x00
	xor esi, esi
	mov [cpu_info.levels], eax
	mov [cpu_info.vendor+0x00], ebx
	mov [cpu_info.vendor+0x08], ecx
	mov [cpu_info.vendor+0x04], edx
	mov [cpu_info.vendor+0x0C], esi

	mov si, .msg1
	call puts
	mov si, cpu_info.vendor
	call puts
	call putnl

	; Get CPU features
	cpuid 0x01
	mov [cpu_info.features+0x00], eax
	mov [cpu_info.features+0x04], ebx
	mov [cpu_info.features+0x08], ecx
	mov [cpu_info.features+0x0C], edx
	xor cx, cx

.loop:
	mov al, cl
	add al, 0x41
	mov byte [.msg2+0x0C], al
	mov si, .msg2
	call puts

	mov eax, ecx
	shl eax, 0x02
	mov ebx, [cpu_info.features+eax]
	call putx32
	call putnl

	inc cx
	cmp cx, 0x04
	jl .loop

.done:
	popfd
	popad
	ret

.skip:
	xor eax, eax
	mov [cpu_info.levels], eax
	jmp .done

.msg0: db "CPUID detected!", endl, 0x00
.msg1: db "    Vendor id: ", 0x00
.msg2: db "    Feature _: ", 0x00

; Checks if CPUID instruction is available
; Returns:
;   zf    - Unset if doesn't exist
; References:
;   https://wiki.osdev.org/CPUID#Checking_CPUID_availability
[bits 16]
cpuid_exist:
	push eax
	pushfd
	pushfd
	xor dword [esp], 0x00200000
	popfd
	pushfd
	pop eax
	xor eax, [esp]
	popfd
	test eax, 0x00200000
	pop eax
	ret

[global cpu_info]
section .bss
cpu_info:
	.levels:   resd 0x01
	.vendor:   resb 0x0D
	.features: resd 0x04
section .text
