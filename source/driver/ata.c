#include <global.h>
#include "port.h"
#include "ata.h"

#define ATA_COMMAND_IDENTIFY      0xEC
#define ATA_COMMAND_READ28        0x20
#define ATA_COMMAND_WRITE28       0x30
#define ATA_COMMAND_FLUSH28       0xE7
#define ATA_COMMAND_READ48        0x24
#define ATA_COMMAND_WRITE48       0x34
#define ATA_COMMAND_FLUSH48       0xEA

#define ATA_PORT_DATA(Base)       ((Base) + 0x00)
#define ATA_PORT_ERROR(Base)      ((Base) + 0x01)
#define ATA_PORT_FEATURES(Base)   ((Base) + 0x01)
#define ATA_PORT_SECTORS(Base)    ((Base) + 0x02)
#define ATA_PORT_ADDRESS_LO(Base) ((Base) + 0x03)
#define ATA_PORT_ADDRESS_MI(Base) ((Base) + 0x04)
#define ATA_PORT_ADDRESS_HI(Base) ((Base) + 0x05)
#define ATA_PORT_DEVICE(Base)     ((Base) + 0x06)
#define ATA_PORT_STATUS(Base)     ((Base) + 0x07)
#define ATA_PORT_COMMAND(Base)    ((Base) + 0x07)
#define ATA_PORT_CONTROL(Base)    ((Base) + 0x0206)

bool ata_init(uint16_t base, bool master) {
	port8_out(ATA_PORT_DEVICE(base), master ? 0xA0 : 0xB0);
	port8_out(ATA_PORT_CONTROL(base), 0x00);
	port8_out(ATA_PORT_DEVICE(base), 0xA0);
	if (port8_in(ATA_PORT_STATUS(base)) == 0xFF) {
		return false;
	}

	port8_out(ATA_PORT_DEVICE(base), master ? 0xA0 : 0xB0);
	port8_out(ATA_PORT_SECTORS(base), 0x00);
	port8_out(ATA_PORT_ADDRESS_LO(base), 0x00);
	port8_out(ATA_PORT_ADDRESS_MI(base), 0x00);
	port8_out(ATA_PORT_ADDRESS_HI(base), 0x00);
	port8_out(ATA_PORT_COMMAND(base), ATA_COMMAND_IDENTIFY);

	uint8_t status;
	while (((status = port8_in(ATA_PORT_STATUS(base))) & 0x81) == 0x80);
	if (status == 0x00 || status & 0x01) {
		return false;
	}
	for (uint16_t i = 0x00; i < 0x100; i++) {
		port16_in(ATA_PORT_DATA(base));
	}
	return true;
}

static uint16_t ata_read28(uint16_t base, bool master, uint32_t address, void *buffer, uint16_t sectors) {
	if (address & 0xF0000000) {
		return 0x00;
	}

	port8_out(ATA_PORT_DEVICE(base), (master ? 0xE0 : 0xF0) | ((address >> 0x18) & 0x0F));
	port8_out(ATA_PORT_ERROR(base), 0x00);
	port8_out(ATA_PORT_SECTORS(base), sectors > UINT8_MAX ? 0x00 : sectors);

	port8_out(ATA_PORT_ADDRESS_LO(base), address >> 0x00);
	port8_out(ATA_PORT_ADDRESS_MI(base), address >> 0x08);
	port8_out(ATA_PORT_ADDRESS_HI(base), address >> 0x10);

	port8_out(ATA_PORT_COMMAND(base), ATA_COMMAND_READ28);
	for (uint16_t i = 0; i < MAX(sectors, UINT8_MAX + 0x01); i++) {
		uint8_t status;
		while (((status = port8_in(ATA_PORT_STATUS(base))) & 0x81) == 0x80);
		if (status == 0x00 || status & 0x01) {
			return i;
		}
		for (uint16_t j = 0x00; j < 0x100; j++) {
			*((uint16_t*)buffer) = port16_in(ATA_PORT_DATA(base));
			buffer = ((uint16_t*)buffer) + 1;
		}
	}
	return sectors;
}

static uint16_t ata_write28(uint16_t base, bool master, uint32_t address, const void *buffer, uint16_t sectors) {
	if (address & 0xF0000000) {
		return 0x00;
	}

	port8_out(ATA_PORT_DEVICE(base), (master ? 0xE0 : 0xF0) | ((address >> 0x18) & 0x0F));
	port8_out(ATA_PORT_ERROR(base), 0x00);
	port8_out(ATA_PORT_SECTORS(base), sectors > UINT8_MAX ? 0x00 : sectors);

	port8_out(ATA_PORT_ADDRESS_LO(base), address >> 0x00);
	port8_out(ATA_PORT_ADDRESS_MI(base), address >> 0x08);
	port8_out(ATA_PORT_ADDRESS_HI(base), address >> 0x10);

	port8_out(ATA_PORT_COMMAND(base), ATA_COMMAND_WRITE28);
	for (uint16_t i = 0x00; i < MAX(sectors, UINT8_MAX + 0x01); i++) {
		uint8_t status;
		while (((status = port8_in(ATA_PORT_STATUS(base))) & 0x81) == 0x80);
		if (status == 0x00 || status & 0x01) {
			return i;
		}
		for (uint16_t j = 0x00; j < 0x100; j++) {
			port16_out(ATA_PORT_DATA(base), *((uint16_t*)buffer));
			buffer = ((uint16_t*)buffer) + 1;
		}
		port8_out(ATA_PORT_COMMAND(base), ATA_COMMAND_FLUSH28);
	}
	return sectors;
}
static uint32_t ata_read48(uint16_t base, bool master, uint64_t address, void *buffer, uint32_t sectors) {
	if (address & 0xFFFF000000000000) {
		return 0x00;
	}

	port8_out(ATA_PORT_DEVICE(base), master ? 0x40 : 0x50);
	port8_out(ATA_PORT_ERROR(base), 0x00);

	port8_out(ATA_PORT_SECTORS(base), sectors > UINT16_MAX ? 0x00 : (sectors >> 0x08));
	port8_out(ATA_PORT_ADDRESS_LO(base), address >> 0x18);
	port8_out(ATA_PORT_ADDRESS_MI(base), address >> 0x20);
	port8_out(ATA_PORT_ADDRESS_HI(base), address >> 0x28);

	port8_out(ATA_PORT_SECTORS(base), sectors > UINT16_MAX ? 0x00 : (sectors >> 0x00));
	port8_out(ATA_PORT_ADDRESS_LO(base), address >> 0x00);
	port8_out(ATA_PORT_ADDRESS_MI(base), address >> 0x08);
	port8_out(ATA_PORT_ADDRESS_HI(base), address >> 0x10);

	port8_out(ATA_PORT_COMMAND(base), ATA_COMMAND_READ48);
	for (uint32_t i = 0x00; i < MAX(sectors, UINT16_MAX + 0x01); i++) {
		uint8_t status;
		while (((status = port8_in(ATA_PORT_STATUS(base))) & 0x81) == 0x80);
		if (status == 0x00 || status & 0x01) {
			return i;
		}
		for (uint16_t j = 0x00; j < 0x100; j++) {
			*((uint16_t*)buffer) = port16_in(ATA_PORT_DATA(base));
			buffer = ((uint16_t*)buffer) + 1;
		}
	}
	return sectors;
}

static uint32_t ata_write48(uint16_t base, bool master, uint64_t address, const void *buffer, uint32_t sectors) {
	if (address & 0xFFFF000000000000) {
		return 0x00;
	}

	port8_out(ATA_PORT_DEVICE(base), master ? 0x40 : 0x50);
	port8_out(ATA_PORT_ERROR(base), 0x00);

	port8_out(ATA_PORT_SECTORS(base), sectors > UINT16_MAX ? 0x00 : (sectors >> 0x08));
	port8_out(ATA_PORT_ADDRESS_LO(base), address >> 0x18);
	port8_out(ATA_PORT_ADDRESS_MI(base), address >> 0x20);
	port8_out(ATA_PORT_ADDRESS_HI(base), address >> 0x28);

	port8_out(ATA_PORT_SECTORS(base), sectors > UINT16_MAX ? 0x00 : (sectors >> 0x00));
	port8_out(ATA_PORT_ADDRESS_LO(base), address >> 0x00);
	port8_out(ATA_PORT_ADDRESS_MI(base), address >> 0x08);
	port8_out(ATA_PORT_ADDRESS_HI(base), address >> 0x10);

	port8_out(ATA_PORT_COMMAND(base), ATA_COMMAND_WRITE48);
	for (uint32_t i = 0x00; i < MAX(sectors, UINT16_MAX + 0x01); i++) {
		uint8_t status;
		while (((status = port8_in(ATA_PORT_STATUS(base))) & 0x81) == 0x80);
		if (status == 0x00 || status & 0x01) {
			return i;
		}
		for (uint16_t j = 0x00; j < 0x100; j++) {
			port16_out(ATA_PORT_DATA(base), *((uint16_t*)buffer));
			buffer = ((uint16_t*)buffer) + 1;
		}
		port8_out(ATA_PORT_COMMAND(base), ATA_COMMAND_FLUSH48);
	}
	return sectors;
}

uint32_t ata_read(uint16_t base, bool master, uint64_t address, void *buffer, uint32_t sectors) {
	if (~(UINT64_MAX >> 0x24) & address || sectors > UINT8_MAX + 0x01) {
		return ata_read48(base, master, address, buffer, sectors);
	}
	return ata_read28(base, master, address, buffer, sectors);
}

uint32_t ata_write(uint16_t base, bool master, uint64_t address, const void *buffer, uint32_t sectors) {
	if (~(UINT64_MAX >> 0x24) & address || sectors > UINT8_MAX + 0x01) {
		return ata_write48(base, master, address, buffer, sectors);
	}
	return ata_write28(base, master, address, buffer, sectors);
}
