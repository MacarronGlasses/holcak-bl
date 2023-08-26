; Initializes disk
; Parameters:
;   dl    - Disk number
; References:
;   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=41h:_Check_Extensions_Present
;   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=08h:_Read_Drive_Parameters
[bits 16]
disk_init:
	pushad
	mov ah, 0x41
	mov bx, 0x55AA
	xor cx, cx
	int 0x13
	mov [disk_info.extensions], cx

.continue:
	mov ah, 0x08
	int 0x13
	jc .error

	inc dh
	mov [disk_info.heads], dh
	mov bl, cl
	and bl, 0x3F
	mov [disk_info.sectors], bl
	shr cx, 0x06
	inc cx
	mov [disk_info.cylinders], cx

	popad
	mov [disk_info.booting], dl
	ret

.error:
	mov al, 'B'
	jmp pute

; Resets disk controller
; Parameters:
;   dl    - Disk number
[bits 16]
disk_reset:
	push ax
	xor ah, ah
	int 0x13
	jc .error
	pop ax
	ret

.error:
	mov al, 'C'
	jmp pute

; Reads sectors from disk
; Parameters:
;   eax   - LBA pointing to the beginning of the data
;   ebx   - Address of buffer [offset 16] [segment 16]
;   si    - Number of sectors to read
;   dl    - Disk number
[bits 16]
disk_read:
	push cx
	mov cx, [disk_info.extensions]
	test cx, 0x01
	pop cx
	jz disk_read_0x02
	jmp disk_read_0x42

; Reads sectors from disk using int 0x13, ah=0x02
; Parameters:
;   eax   - LBA pointing to the beginning of the data
;   ebx   - Address of buffer [offset 16] [segment 16]
;   si    - Number of sectors to read
;   dl    - Disk number
; References:
;   https://stanislavs.org/helppc/int_13-2.html
[bits 16]
disk_read_0x02:
	pushad
	push es

.loop:
	push eax
	push ebx
	push dx

	; Convert LBA to CHS
	xor edx, edx
	xor ebx, ebx
	mov bl, [disk_info.sectors]
	div ebx

	inc dl
	mov cl, dl

	xor edx, edx
	mov bl, [disk_info.heads]
	div ebx

	mov dh, dl
	mov ch, al
	shl ah, 0x06
	or cl, ah

	; Initialize es:bx (Pointer to buffer)
	mov ebx, [esp+0x02]
	mov di, bx
	and di, 0x0F
	shr ebx, 0x10
	mov es, bx
	mov bx, di

	; Initialize al (Number of sectors)
	mov al, 0x80
	cmp si, 0x80
	jge .retry
	mov ax, si

	; Restore disk number
	pop di
	xor dl, dl
	or dx, di

	; Read sectors from disk
	mov di, 0x05

.retry:
	mov ah, 0x02
	push ax

	int 0x13
	jc .error

	xor ecx, ecx
	mov cl, al
	pop ax

	pop ebx
	pop eax

	sub si, cx
	test si, si
	jz .done
	add eax, ecx
	add ebx, ecx
	jmp .loop

.error:
	test di, di
	jz .fatal
	call disk_reset
	dec di
	pop ax
	jmp .retry

.fatal:
	mov al, 'D'
	jmp pute

.done:
	pop es
	popad
	ret

section .bss
disk_info:
	.extensions: resb 0x01
	.cylinders:  resw 0x01
	.heads:      resb 0x01
	.sectors:    resb 0x01
	.booting:    resb 0x01
section .text

; Reads sectors from disk
; Parameters:
;   eax   - LBA pointing to the beginning of the data
;   ebx   - Address of buffer [offset 16] [segment 16]
;   si    - Number of sectors to read
;   dl    - Disk number
; References:
;   http://www.ctyme.com/intr/rb-0708.htm
;   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=42h:_Extended_Read_Sectors_From_Drive
[bits 16]
disk_read_0x42:
	push ax
	push si

	mov word  [disk_packet.sectors],  si
	mov dword [disk_packet.buffer],   ebx
	mov dword [disk_packet.lba_lo],   eax

	mov si, disk_packet
	mov ah, 0x42
	int 0x13
	jc .error

	pop si
	pop ax
	ret

.error:
	mov al, 'E'
	jmp pute

disk_packet:
	.size:     db 0x10
	.reserved: db 0x00
	.sectors:  dw 0x00
	.buffer:   dd 0x00
	.lba_lo:   dd 0x00
	.lba_hi:   dd 0x00
