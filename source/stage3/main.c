#include <stdnoreturn.h>
#include <driver/idt.h>
#include <driver/isr.h>
#include <global.h>
#include "printf.h"
#include "info.h"

noreturn void error(isr_frame_t frame) {
	const char *msg[32] = {"Division Error", "Debug", "Non-Maskable Interrupt", "Breakpoint", "Overflow", "Bound Range Exceeded", "Invalid Opcode", "Device Not Available", "Double Fault", "Coprocessor Segment Overrun", "Invalid TSS", "Segment Not Present", "Stack-Segment Fault", "General Protection Fault", "Page Fault", "Reserved", "x87 Floating-Point Exception", "Alignment Check", "Machine Check", "SIMD Floating-Point Exception", "Control Protection Exception", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Hypervisor Injection Exception", "VMM Comunication Exception", "Security Exception", "Reserved"};
	printf("Error detected!\n");
	printf("    Message:   %s\n", msg[frame.id]);
	printf("    Error:     %x\n", frame.error);
	printf("    EAX:       %x\n", frame.eax);
	printf("    EBX:       %x\n", frame.ebx);
	printf("    ECX:       %x\n", frame.ecx);
	printf("    EDX:       %x\n", frame.edx);
	printf("    ESP:       %x\n", frame.esp);
	printf("    EBP:       %x\n", frame.ebp);
	printf("    ESI:       %x\n", frame.esi);
	printf("    EDI:       %x\n", frame.edi);
	printf("    EIP:       %x\n", frame.eip);
	printf("    DS:        %x\n", frame.ds);
	printf("    CS:        %x\n", frame.cs);
	printf("    SS:        %x\n", frame.ss);
	printf("    FLAGS:     %x\n", frame.flags);
	__asm__ volatile ("cli");
	while (1);
}

__cdecl void main(void) {
	printf("GDT enabled!\n");
	isr_init();
	for (uint8_t i = 0; i < 32; i++) {
		isr_handler_init(i, error);
	}
	idt_init();
	printf("IDT enabled!\n");
}
