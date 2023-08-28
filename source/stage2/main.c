#include <stdint.h>
#include <define.h>
#include "cpuid.h"
#include "puts.h"
#include "pci.h"

__cdecl void start_16(uint32_t disk) {
	// TODO: Pass all the info to stage3
//	__attribute__((far)) void *x;

//	cpu_info_t *cpu;
	if (cpuid_exist()) {
		printf("CPUID detected!\n\r");
		// TODO: Initialize cpu specific features!
	}

	pci_info_t *pci = pci_init();
	if (pci->mechanism != PCI_MECHANISM_NOT_FOUND) {
		printf("PCI detected!\n\r");
	}
}
