#include "isr.h"
#include "idt.h"

static idt_gate_t idt_table[0x0100];
static idt_desc_t idt_desc = {0, 0};

void idt_init(void) {
	idt_desc = (idt_desc_t){sizeof(idt_table)-1, (uint32_t)idt_table};
	__asm__ volatile ("lidtl (%0)" : : "r" (&idt_desc));
}

void idt_gate_init(uint8_t id, void(*base)(void), uint8_t flags) {
	idt_table[id] = (idt_gate_t){(uint32_t)base >> 0x00, 0x08, 0x00, flags, (uint32_t)base >> 0x10};
}
