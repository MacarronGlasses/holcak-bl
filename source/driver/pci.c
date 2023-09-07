#include "port.h"
#include "pci.h"

uint32_t pci_read(uint16_t id, uint8_t offset) {
	port32_out(PCI_CONFIG_ADDRESS, ((uint32_t)id << 0x08) | ((uint32_t)offset) | (0x01 << 0x1F));
	return port32_in(PCI_CONFIG_DATA);
}

void pci_write(uint16_t id, uint8_t offset, uint32_t value) {
	port32_out(PCI_CONFIG_ADDRESS, ((uint32_t)id << 0x08) | ((uint32_t)offset) | (0x01 << 0x1F));
	port32_out(PCI_CONFIG_DATA, value);
}

uint16_t pci_find(uint16_t class, uint16_t start, uint16_t limit) {
	for (; start < limit; start++) {
		if ((pci_read(start, 0x08) >> 0x10) == class) {
			return start;
		}
		if ((start & 0x07) == 0x00 && !(pci_read(start, 0x0C) & 0x01 << 0x17)) {
			start = (start + 0x08) & ~0x07;
		}
	}
	return UINT16_MAX;
}
