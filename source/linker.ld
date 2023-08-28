ENTRY(start_16)

SECTIONS
{
	. = 0x1000;

	.entry  : {*(.entry) }
	.text   : {*(.text)  }
	.data   : {*(.data)  }
	.rodata : {*(.rodata)}
	.bss    : {*(.bss)   }
}
