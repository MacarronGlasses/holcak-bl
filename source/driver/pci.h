#ifndef DRIVER_PCI_H_
#define DRIVER_PCI_H_
#include <stdint.h>

#define PCI_CONFIG_ADDRESS 0x0CF8
#define PCI_CONFIG_DATA    0x0CFC
#define PCI_INVALID_VENDOR 0xFFFF

#define PCI_ID(Bus, Device, Function) ((((uint16_t)(Bus) & 0xFF) << 0x08) | (((uint16_t)(Device) & 0x1F) << 0x03) | (((uint16_t)(Function) & 0x07) << 0x00))

uint32_t pci_read(uint16_t id, uint8_t offset);
void pci_write(uint16_t id, uint8_t offset, uint32_t value);
uint16_t pci_find(uint16_t class, uint16_t start, uint16_t limit);

#endif//DRIVER_PCI_H_
