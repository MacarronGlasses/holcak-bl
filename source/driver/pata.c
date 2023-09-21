#include <global.h>
#include "port.h"
#include "pata.h"

#define PATA_COMMAND_IDENTIFY      0xEC
#define PATA_COMMAND_READ28        0x20
#define PATA_COMMAND_WRITE28       0x30
#define PATA_COMMAND_FLUSH28       0xE7
#define PATA_COMMAND_READ48        0x24
#define PATA_COMMAND_WRITE48       0x34
#define PATA_COMMAND_FLUSH48       0xEA

#define PATA_PORT_DPATA(Base)      ((Base) + 0x00)
#define PATA_PORT_ERROR(Base)      ((Base) + 0x01)
#define PATA_PORT_FEATURES(Base)   ((Base) + 0x01)
#define PATA_PORT_SECTORS(Base)    ((Base) + 0x02)
#define PATA_PORT_ADDRESS_LO(Base) ((Base) + 0x03)
#define PATA_PORT_ADDRESS_MI(Base) ((Base) + 0x04)
#define PATA_PORT_ADDRESS_HI(Base) ((Base) + 0x05)
#define PATA_PORT_DEVICE(Base)     ((Base) + 0x06)
#define PATA_PORT_STATUS(Base)     ((Base) + 0x07)
#define PATA_PORT_COMMAND(Base)    ((Base) + 0x07)
#define PATA_PORT_CONTROL(Base)    ((Base) + 0x0206)

bool pata_init(pata_info_t info) {
	port8_out(PATA_PORT_DEVICE(info.base), info.master ? 0xA0 : 0xB0);
	port8_out(PATA_PORT_CONTROL(info.base), 0x00);
	port8_out(PATA_PORT_DEVICE(info.base), 0xA0);
	if (port8_in(PATA_PORT_STATUS(info.base)) == 0xFF) {
		return false;
	}

	port8_out(PATA_PORT_DEVICE(info.base), info.master ? 0xA0 : 0xB0);
	port8_out(PATA_PORT_SECTORS(info.base), 0x00);
	port8_out(PATA_PORT_ADDRESS_LO(info.base), 0x00);
	port8_out(PATA_PORT_ADDRESS_MI(info.base), 0x00);
	port8_out(PATA_PORT_ADDRESS_HI(info.base), 0x00);
	port8_out(PATA_PORT_COMMAND(info.base), PATA_COMMAND_IDENTIFY);

	uint8_t status;
	while (((status = port8_in(PATA_PORT_STATUS(info.base))) & 0x81) == 0x80);
	if (status == 0x00 || status & 0x01) {
		return false;
	}
	for (uint16_t i = 0x00; i < 0x100; i++) {
		port16_in(PATA_PORT_DPATA(info.base));
	}
	return true;
}

static uint16_t pata_read28(pata_info_t info, uint32_t address, void *buffer, uint16_t sectors) {
	if (address & 0xF0000000) {
		return 0x00;
	}

	port8_out(PATA_PORT_DEVICE(info.base), (info.master ? 0xE0 : 0xF0) | ((address >> 0x18) & 0x0F));
	port8_out(PATA_PORT_ERROR(info.base), 0x00);
	port8_out(PATA_PORT_SECTORS(info.base), sectors > UINT8_MAX ? 0x00 : sectors);

	port8_out(PATA_PORT_ADDRESS_LO(info.base), address >> 0x00);
	port8_out(PATA_PORT_ADDRESS_MI(info.base), address >> 0x08);
	port8_out(PATA_PORT_ADDRESS_HI(info.base), address >> 0x10);

	port8_out(PATA_PORT_COMMAND(info.base), PATA_COMMAND_READ28);
	for (uint16_t i = 0; i < MAX(sectors, UINT8_MAX + 0x01); i++) {
		uint8_t status;
		while (((status = port8_in(PATA_PORT_STATUS(info.base))) & 0x81) == 0x80);
		if (status == 0x00 || status & 0x01) {
			return i;
		}
		for (uint16_t j = 0x00; j < 0x100; j++) {
			*((uint16_t*)buffer) = port16_in(PATA_PORT_DPATA(info.base));
			buffer = ((uint16_t*)buffer) + 1;
		}
	}
	return sectors;
}

static uint16_t pata_write28(pata_info_t info, uint32_t address, const void *buffer, uint16_t sectors) {
	if (address & 0xF0000000) {
		return 0x00;
	}

	port8_out(PATA_PORT_DEVICE(info.base), (info.master ? 0xE0 : 0xF0) | ((address >> 0x18) & 0x0F));
	port8_out(PATA_PORT_ERROR(info.base), 0x00);
	port8_out(PATA_PORT_SECTORS(info.base), sectors > UINT8_MAX ? 0x00 : sectors);

	port8_out(PATA_PORT_ADDRESS_LO(info.base), address >> 0x00);
	port8_out(PATA_PORT_ADDRESS_MI(info.base), address >> 0x08);
	port8_out(PATA_PORT_ADDRESS_HI(info.base), address >> 0x10);

	port8_out(PATA_PORT_COMMAND(info.base), PATA_COMMAND_WRITE28);
	for (uint16_t i = 0x00; i < MAX(sectors, UINT8_MAX + 0x01); i++) {
		uint8_t status;
		while (((status = port8_in(PATA_PORT_STATUS(info.base))) & 0x81) == 0x80);
		if (status == 0x00 || status & 0x01) {
			return i;
		}
		for (uint16_t j = 0x00; j < 0x100; j++) {
			port16_out(PATA_PORT_DPATA(info.base), *((uint16_t*)buffer));
			buffer = ((uint16_t*)buffer) + 1;
		}
		port8_out(PATA_PORT_COMMAND(info.base), PATA_COMMAND_FLUSH28);
	}
	return sectors;
}
static uint32_t pata_read48(pata_info_t info, uint64_t address, void *buffer, uint32_t sectors) {
	if (address & 0xFFFF000000000000) {
		return 0x00;
	}

	port8_out(PATA_PORT_DEVICE(info.base), info.master ? 0x40 : 0x50);
	port8_out(PATA_PORT_ERROR(info.base), 0x00);

	port8_out(PATA_PORT_SECTORS(info.base), sectors > UINT16_MAX ? 0x00 : (sectors >> 0x08));
	port8_out(PATA_PORT_ADDRESS_LO(info.base), address >> 0x18);
	port8_out(PATA_PORT_ADDRESS_MI(info.base), address >> 0x20);
	port8_out(PATA_PORT_ADDRESS_HI(info.base), address >> 0x28);

	port8_out(PATA_PORT_SECTORS(info.base), sectors > UINT16_MAX ? 0x00 : (sectors >> 0x00));
	port8_out(PATA_PORT_ADDRESS_LO(info.base), address >> 0x00);
	port8_out(PATA_PORT_ADDRESS_MI(info.base), address >> 0x08);
	port8_out(PATA_PORT_ADDRESS_HI(info.base), address >> 0x10);

	port8_out(PATA_PORT_COMMAND(info.base), PATA_COMMAND_READ48);
	for (uint32_t i = 0x00; i < MAX(sectors, UINT16_MAX + 0x01); i++) {
		uint8_t status;
		while (((status = port8_in(PATA_PORT_STATUS(info.base))) & 0x81) == 0x80);
		if (status == 0x00 || status & 0x01) {
			return i;
		}
		for (uint16_t j = 0x00; j < 0x100; j++) {
			*((uint16_t*)buffer) = port16_in(PATA_PORT_DPATA(info.base));
			buffer = ((uint16_t*)buffer) + 1;
		}
	}
	return sectors;
}

static uint32_t pata_write48(pata_info_t info, uint64_t address, const void *buffer, uint32_t sectors) {
	if (address & 0xFFFF000000000000) {
		return 0x00;
	}

	port8_out(PATA_PORT_DEVICE(info.base), info.master ? 0x40 : 0x50);
	port8_out(PATA_PORT_ERROR(info.base), 0x00);

	port8_out(PATA_PORT_SECTORS(info.base), sectors > UINT16_MAX ? 0x00 : (sectors >> 0x08));
	port8_out(PATA_PORT_ADDRESS_LO(info.base), address >> 0x18);
	port8_out(PATA_PORT_ADDRESS_MI(info.base), address >> 0x20);
	port8_out(PATA_PORT_ADDRESS_HI(info.base), address >> 0x28);

	port8_out(PATA_PORT_SECTORS(info.base), sectors > UINT16_MAX ? 0x00 : (sectors >> 0x00));
	port8_out(PATA_PORT_ADDRESS_LO(info.base), address >> 0x00);
	port8_out(PATA_PORT_ADDRESS_MI(info.base), address >> 0x08);
	port8_out(PATA_PORT_ADDRESS_HI(info.base), address >> 0x10);

	port8_out(PATA_PORT_COMMAND(info.base), PATA_COMMAND_WRITE48);
	for (uint32_t i = 0x00; i < MAX(sectors, UINT16_MAX + 0x01); i++) {
		uint8_t status;
		while (((status = port8_in(PATA_PORT_STATUS(info.base))) & 0x81) == 0x80);
		if (status == 0x00 || status & 0x01) {
			return i;
		}
		for (uint16_t j = 0x00; j < 0x100; j++) {
			port16_out(PATA_PORT_DPATA(info.base), *((uint16_t*)buffer));
			buffer = ((uint16_t*)buffer) + 1;
		}
		port8_out(PATA_PORT_COMMAND(info.base), PATA_COMMAND_FLUSH48);
	}
	return sectors;
}

uint32_t pata_read(pata_info_t info, uint64_t address, void *buffer, uint32_t sectors) {
	if (~(UINT64_MAX >> 0x24) & address || sectors > UINT8_MAX + 0x01) {
		return pata_read48(info, address, buffer, sectors);
	}
	return pata_read28(info, address, buffer, sectors);
}

uint32_t pata_write(pata_info_t info, uint64_t address, const void *buffer, uint32_t sectors) {
	if (~(UINT64_MAX >> 0x24) & address || sectors > UINT8_MAX + 0x01) {
		return pata_write48(info, address, buffer, sectors);
	}
	return pata_write28(info, address, buffer, sectors);
}
