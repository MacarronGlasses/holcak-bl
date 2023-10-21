#!/bin/python3
from sys import argv

if len(argv) < 2:
	print(f"Usage: {argv[0]} <folder>")
	print("Error: Insufficient number of arguments were provided!")
	exit(1)
with open(f"{argv[1]}/driver/isrg.h", "w") as f:
	f.write("#pragma once\n")
	f.write("#include <global.h>\n")
	f.write("#include \"idt.h\"\n\n")
	for i in range(0xFF):
		f.write(f"extern __cdecl void isr_{i}(void);\n")
	f.write("void isr_init(void);\n")
with open(f"{argv[1]}/driver/isrg.c", "w") as f:
	f.write("#include \"isrg.h\"\n\n")
	f.write("void isr_init(void) {\n")
	for i in range(0xFF):
		f.write(f"\tidt_gate_init({i}, isr_{i}, IDT_PRESENT | IDT_GATE_INT_32 | IDT_RING_0);\n")
	f.write("}\n")
