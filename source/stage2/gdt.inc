gdt_info:
.null:
	dd 0x00
	dd 0x00
.code:
	dw 0xFFFF
	dw 0x00
	db 0x00
	db 0x9A
	db 0xCF
	db 0x00
.data:
	dw 0xFFFF
	dw 0x00
	db 0x00
	db 0x92
	db 0xCF
	db 0x00
.desc:
	dw .desc - gdt_info - 1
	dw gdt_info

code_seg: equ gdt_info.code - gdt_info
data_seg: equ gdt_info.data - gdt_info