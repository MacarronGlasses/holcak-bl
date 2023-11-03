#include <stdnoreturn.h>
#include <driver/disk.h>
#include <driver/idt.h>
#include <driver/isr.h>
#include <driver/pic.h>
#include <driver/pci.h>
#include <driver/mbr.h>
#include <driver/mem.h>
#include <driver/mb1.h>
#include <driver/pmm.h>
#include <global.h>
#include "printf.h"
#include "info.h"

extern mem_info_t mem_info;

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
	printf("Exception detected!\n    String:    %s\n    Extended:  %x\n    EAX:       %x\n    EBX:       %x\n    ECX:       %x\n    EDX:       %x\n    ESP:       %x\n    EBP:       %x\n    ESI:       %x\n    EDI:       %x\n    EIP:       %x\n    DS:        %x\n    CS:        %x\n    SS:        %x\n    FLAGS:     %x\n", msg[frame.id], frame.error, frame.eax, frame.ebx, frame.ecx, frame.edx, frame.esp, frame.ebp, frame.esi, frame.edi, frame.eip, frame.ds, frame.cs, frame.ss, frame.flags);
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
	pic_init();
	printf("PIC enabled!\n");
	if (!pmm_init(&mem_info)) {
		panic("Error: Could not initialize PMM!\n");
	}
	printf("PMM enabled!\n");
	for (size_t i = 0; i < 0x0100; i++) {
		// TODO: It is returning NULL, we need to filter out the mem_info!
		printf("%p\n", pmm_alloc());
	}
	pmm_dump();

	/*
	disk_t disk = disk_pata(0x01F0, true);
	if (disk.sectors == 0x00) {
		panic("Error: Could not initialize disk!");
	}
	char buffer[0x0200];
	if (disk_read(&disk, 0x00, buffer, 0x01) != 0x01) {
		panic("Error: Could not read from disk!");
	}
	buffer[0x0C] = '\0';
	printf("%s\n", &buffer[0x03]);
	*/

	/*
	for (uint16_t id = 0x00; id < PCI_ID(pci_info.buses, 0x00, 0x00); ) {
		if ((pci_read(id, 0x00) & UINT16_MAX) == UINT16_MAX) {
			id += 0x08;
			continue;
		}
		bool skip = !((pci_read(id, 0x0C) >> 0x10) & 0x80);
		for (uint16_t to = id + (skip ? 0x01 : 0x08); id < to; id++) {
			uint32_t info = pci_read(id, 0x08);
			switch (info >> 0x10) {
				case 0x0101: {
					printf("IDE detected!\n");
					printf("    Interface: %hhx\n", info >> 0x08);
					if ((info >> 0x08) & 0x01) {
						panic("Error: IDE PCI native mode isn't implemented yet!");
					} else for (uint8_t i = 0x00; i < 0x04; i++) {
						if (!pata_init(i & 0x02 ? 0x01F0 : 0x0170, i & 0x01)) {
							continue;
						}
						printf("PATA detected!\n");
						printf("    Address:   %hx\n", i & 0x02 ? 0x01F0 : 0x0170);
						printf("    Master:    %hx\n", i & 0x01);
					}
				} break;
				case 0x0106: {
					panic("Error: SATA isn't implemented yet!");
				} break;
			}
		}
		id += skip ? 0x07 : 0x00;
	}
	*/
}
