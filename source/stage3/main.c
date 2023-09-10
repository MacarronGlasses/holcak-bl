#include <stdnoreturn.h>
#include <driver/idt.h>
#include <driver/isr.h>
#include <driver/pic.h>
#include <driver/pci.h>
#include <driver/ata.h>
#include <driver/mbr.h>
#include <global.h>
#include "printf.h"
#include "info.h"

// TODO: https://wiki.osdev.org/PCI_IDE_Controller

noreturn void panic(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	__asm__ volatile ("cli");
	while (1);
}

noreturn void error(isr_frame_t frame) {
	const char *msg[0x20] = {"Division Error", "Debug", "Non-Maskable Interrupt", "Breakpoint", "Overflow", "Bound Range Exceeded", "Invalid Opcode", "Device Not Available", "Double Fault", "Coprocessor Segment Overrun", "Invalid TSS", "Segment Not Present", "Stack-Segment Fault", "General Protection Fault", "Page Fault", "Reserved", "x87 Floating-Point Exception", "Alignment Check", "Machine Check", "SIMD Floating-Point Exception", "Control Protection Exception", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Hypervisor Injection Exception", "VMM Comunication Exception", "Security Exception", "Reserved"};
	printf("Error detected!\n    Message:   %s\n    Error:     %x\n    EAX:       %x\n    EBX:       %x\n    ECX:       %x\n    EDX:       %x\n    ESP:       %x\n    EBP:       %x\n    ESI:       %x\n    EDI:       %x\n    EIP:       %x\n    DS:        %x\n    CS:        %x\n    SS:        %x\n    FLAGS:     %x\n", msg[frame.id], frame.error, frame.eax, frame.ebx, frame.ecx, frame.edx, frame.esp, frame.ebp, frame.esi, frame.edi, frame.eip, frame.ds, frame.cs, frame.ss, frame.flags);
	__asm__ volatile ("cli");
	while (1);
}

__cdecl void main(void) {
	printf("GDT enabled!\n");
	isr_init();
	for (uint8_t i = 0x00; i < 0x20; i++) {
		isr_handler_init(i, error);
	}
	idt_init();
	printf("IDT enabled!\n");
	mbr_partition_t *partitions = (void*)0x06BE;
	for (uint8_t i = 0; i < 4; i++) {
		printf("MBR partition %hhx\n", i);
		printf("    Bootable: %hhx\n", partitions[i].bootable);
		printf("    SystemID: %hhx\n", partitions[i].system_id);
		printf("    Address:  %x\n", partitions[i].address);
		printf("    Sectors:  %x\n", partitions[i].sectors);
	}
}
