[org 0x7C00]
; TODO: Better error reporting
; TODO: Add LBA 48-bit support

[bits 16]
jmp start_hi
times 0x03-($-$$) db 0x00

; BIOS Parameter Block is needed because some bioses are overwriting this section
; References:	
;   https://en.wikipedia.org/wiki/BIOS_parameter_block#DOS_4.0_EBPB
[bits 16]
start_bdb:
	.oem_id:               db "HOLCAK  "
	.bytes_per_sector:     dw 0x0200
	.sectors_per_cluster:  db 0x00
	.reserved_sectors:     dw 0x00
	.number_of_fat:        db 0x00
	.root_entries:         dw 0x00
	.total_sectors:        dw 0x00
	.media_descriptor:     db 0x00
	.sectors_per_fat:      dw 0x00

	.sectors_per_track:    dw 0x12
	.number_of_heads:      dw 0x02
	.hidden_sectors:       dd 0x00
	.large_total_sectors:  dd 0x00

	.drive_number:         db 0x00
	.flags:                db 0x00
	.boot_signature:       db 0x00
	.volume_serial_number: dd 0x00
	.volume_label:         db "HOLCAK     "
	.file_system_type:     times 0x08 db 0x00

%include "pute.inc"
%include "disk.inc"

[bits 16]
start_hi:
	; Initialize registers
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7C00
	sti

	; Relocate itself to lower address
	cld
	mov cx, 0x50
	mov si, 0x7C00
	mov di, 0x0500
	rep movsd
	jmp 0x0000:start_lo

[bits 16]
start_lo:
	; Load stage2 into memory
	call disk_init
	mov eax, [stage2_location]
	mov ebx, (stage2_buffer << 0x0C) | (stage2_buffer % 0x10)
	mov si,  [stage2_sectors]
	call disk_read

	; Jump to stage2
	jmp stage2_buffer

times 0x01B8-($-$$) db 0x00
stage2_location: dd  0x00
stage2_sectors:  dw  0x00
stage2_buffer:   equ 0x1000

times 0x01BE-($-$$) db 0x00
%macro partition_make 1
partition_%1:
	.bootable:     db 0x00
	.chs_start_lo: db 0x00
	.chs_start_hi: dw 0x00
	.system_id:    db 0x00
	.chs_end_lo:   db 0x00
	.chs_end_hi:   dw 0x00
	.lba_start:    dd 0x00
	.sectors:      dd 0x00
%endmacro

partition_make 1
partition_make 2
partition_make 3
partition_make 4

times 0x01FE-($-$$) db 0x00
dw 0xAA55
