#include "port.h"
#include "pic.h"

void pic_init(void) {
	port8_out(PIC1_COMMAND, PIC_ICW1_INIT | PIC_ICW1_EDGE | PIC_ICW1_ICW4);
	port_wait();
	port8_out(PIC2_COMMAND, PIC_ICW1_INIT | PIC_ICW1_EDGE | PIC_ICW1_ICW4);
	port_wait();

	port8_out(PIC1_DATA, PIC1_OFFSET);
	port_wait();
	port8_out(PIC2_DATA, PIC2_OFFSET);
	port_wait();

	port8_out(PIC1_DATA, 0x04);
	port_wait();
	port8_out(PIC2_DATA, 0x02);
	port_wait();

	port8_out(PIC1_DATA, PIC_ICW4_8086);
	port_wait();
	port8_out(PIC2_DATA, PIC_ICW4_8086);
	port_wait();

	port8_out(PIC1_DATA, 0x00);
	port_wait();
	port8_out(PIC2_DATA, 0x00);
	port_wait();
}

void pic_stop(void) {
	port8_out(PIC2_DATA, 0xFF);
	port8_out(PIC1_DATA, 0xFF);
}

void pic_mask(uint8_t id, bool mask) {
	uint8_t port = id < 0x08 ? PIC1_DATA : (id -= 0x08, PIC2_DATA);
	port8_out(port, mask ? port8_in(port) | (0x01 << id) : port8_in(port) & ~(0x01 << id));
	port_wait();
}

void pic_eoi(uint8_t id) {
	port8_out(id < 0x08 ? PIC1_COMMAND : PIC2_COMMAND, PIC_COMMAND_EOI);
	port_wait();
}

uint16_t pic_irr(void) {
	port8_out(PIC1_COMMAND, PIC_COMMAND_IRR);
	port_wait();
	port8_out(PIC2_COMMAND, PIC_COMMAND_IRR);
	port_wait();

	return port8_in(PIC1_COMMAND) | (port8_in(PIC2_COMMAND) << 0x08);
}

uint16_t pic_isr(void) {
	port8_out(PIC1_COMMAND, PIC_COMMAND_ISR);
	port_wait();
	port8_out(PIC2_COMMAND, PIC_COMMAND_ISR);
	port_wait();

	return port8_in(PIC1_COMMAND) | (port8_in(PIC2_COMMAND) << 0x08);
}
