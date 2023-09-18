#include "port.h"
#include "pci.h"

#define PCI_PORT_ADDRESS 0x0CF8
#define PCI_PORT_DATA    0x0CFC

uint32_t pci_read(uint16_t id, uint8_t offset) {
	port32_out(PCI_PORT_ADDRESS, ((uint32_t)id << 0x08) | ((uint32_t)offset & 0xFC) | (0x01 << 0x1F));
	return port32_in(PCI_PORT_DATA);
}

void pci_write(uint16_t id, uint8_t offset, uint32_t value) {
	port32_out(PCI_PORT_ADDRESS, ((uint32_t)id << 0x08) | ((uint32_t)offset & 0xFC) | (0x01 << 0x1F));
	port32_out(PCI_PORT_DATA, value);
}
