; Detects free regions of memory
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
[bits 16]
mem_init:
	pushad
	push es

	; Initialize registers
	mov eax, 0xE820
	xor ebx, ebx
	mov ecx, mem_node_size
	mov edx, 0x534D4150

	; Initialize es:di (Pointer to buffer)
	mov edi, mem_info.data
	shr edi, 0x04
	mov es, di
	mov edi, mem_info.data
	and di, 0x0F

	; Check if int 0x15, ah=0xE820 is supported
	int 0x15
	cmp eax, edx
	jne .error

	; Save size of `mem_node`
	mov [mem_info.bytes], ecx
	pop es

	call mem_init_0xE820

	; Print debug information	
	mov si, .msg1
	call puts
	xor ebx, ebx
	mov bx, [mem_info.length]
	call putx32
	call putnl

	mov si, .msg2
	call puts
	mov ebx, [mem_info.bytes]
	call putx32
	call putnl

	popad
	ret

.error:
	; TODO: Try different interrupt! https://wiki.osdev.org/Detecting_Memory_(x86)#Other_Methods
	mov si, .msg0
	call puts
	cli
	hlt

.msg0: db "Error: Memory initialization failed!", 0x00
.msg1: db "Memory info length: ", 0x00
.msg2: db "Memory info block:  ", 0x00

; Detects free regions of memory using interrupt 0xE820
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
[bits 16]
mem_init_0xE820:
	pushad

	; Initialize registers
	xor ebx, ebx
	mov edx, 0x534D4150
	mov ecx, [mem_info.bytes]
	mov edi, mem_info.data
	xor esi, esi

.loop:
	; Test if we exceeded maximum length
	cmp esi, mem_data_size
	jge .warning

	; Initialize es:di (Pointer to buffer)
	push edi
	shr edi, 0x04
	mov es, di
	mov edi, [esp+0x00]
	and di, 0x0F

	mov eax, 0xE820
	int 0x15
	jc .error

	; Test if size of block is valid
	mov eax, [mem_info.bytes]
	cmp eax, ecx
	jne .error

	; Add offset to buffer and increment length
	pop edi
	call mem_node_print
	add edi, mem_node_size
	inc si

	test ebx, ebx
	jnz .loop

.done:
	mov [mem_info.length], si
	popad
	ret

.warning:
	push si
	mov si, .msg1
	call puts
	pop si
	jmp .done

.error:
	mov si, .msg0
	call puts
	cli
	hlt

.msg0: db "Error: Memory initialization failed!", 0x00
.msg1: db "Memory info length exceeded maximum!", endl, 0x00

; Prints information about memory node
; Parameters:
;   edi   - Memory block address
mem_node_print:
	push ebx
	push ecx
	push si

	mov si, .msg0
	call puts

	mov ebx, [edi+0x00]
	mov ecx, [edi+0x04]
	call putx64

	mov si, .msg1
	call puts

	mov ebx, [edi+0x08]
	mov ecx, [edi+0x0C]
	call putx64

	mov si, .msg2
	call puts

	mov ebx, [edi+0x10]
	call putx32

	mov si, .msg3
	call puts

	pop si
	pop ecx
	pop ebx
	ret

.msg0: db "Memory detected: ", 0x00
.msg1: db " = ", 0x00
.msg2: db " (", 0x00
.msg3: db ")", endl, 0x00

mem_data_size: equ 0xFF
section .bss
struc mem_node
	.base:  resq 0x01
	.limit: resq 0x01
	.type:  resd 0x01
	.acpi:  resd 0x01
endstruc
mem_info:
	.data:   resb mem_data_size*mem_node_size
	.length: resw 0x01
	.bytes:  resb 0x01
section .text
