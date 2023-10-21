#pragma once
#include <stdbool.h>
#include <global.h>
#include <stdint.h>

#define IDT_GATE_TASK    0x05
#define IDT_GATE_INT_16  0x06
#define IDT_GATE_TRAP_16 0x07
#define IDT_GATE_INT_32  0x0E
#define IDT_GATE_TRAP_32 0x0F
#define IDT_RING_0       0x00
#define IDT_RING_1       0x20
#define IDT_RING_2       0x40
#define IDT_RING_3       0x60
#define IDT_PRESENT      0x80

typedef struct __packed {
	uint16_t base_lo;
	uint16_t segment;
	uint8_t reserved;
	uint8_t flags;
	uint16_t base_hi;
} idt_gate_t;

typedef struct __packed {
	uint16_t limit;
	uint32_t base;
} idt_desc_t;

void idt_init(void);
void idt_gate_init(uint8_t id, void(*base)(void), uint8_t flags);
