#include "port.h"
#include "ata.h"

uint8_t ata_init(uint16_t base, bool master) {
	port8_out(ATA_DEVICE(base), master ? 0xA0 : 0xB0);
	port8_out(ATA_CONTROL(base), 0x00);
	port8_out(ATA_DEVICE(base), 0xA0);
	if (port8_in(ATA_STATUS(base)) == 0xFF) {
		return 0x01;
	}

	port8_out(ATA_DEVICE(base), master ? 0xA0 : 0xB0);
	port8_out(ATA_SECTORS(base), 0x00);
	port8_out(ATA_LBA_LO(base), 0x00);
	port8_out(ATA_LBA_MI(base), 0x00);
	port8_out(ATA_LBA_HI(base), 0x00);
	port8_out(ATA_COMMAND(base), ATA_IDENTIFY);

	uint8_t status;
	while (((status = port8_in(ATA_STATUS(base))) & 0x81) == 0x80);
	if (status == 0x00 || status & 0x01) {
		return 0x02;
	}

	for (uint16_t i = 0x00; i < 0x100; i++) {
		port16_in(ATA_DATA(base));
	}
	return 0x00;
}

uint16_t ata_read28(uint16_t base, bool master, uint32_t address, void *buffer, uint8_t sectors) {
	if (address & 0xF0000000) {
		return 0x00;
	}

	port8_out(ATA_DEVICE(base), (master ? 0xE0 : 0xF0) | ((address >> 0x18) & 0xF));
	port8_out(ATA_ERROR(base), 0x00);
	port8_out(ATA_SECTORS(base), sectors);

	port8_out(ATA_LBA_LO(base), address >> 0x00);
	port8_out(ATA_LBA_MI(base), address >> 0x08);
	port8_out(ATA_LBA_HI(base), address >> 0x10);

	port8_out(ATA_COMMAND(base), ATA_READ28);
	for (uint16_t i = 0; i < (sectors == 0 ? 256 : sectors); i++) {
		uint8_t status;
		while (((status = port8_in(ATA_STATUS(base))) & 0x81) == 0x80);
		if (status == 0x00 || status & 0x01) {
			return i;
		}
		for (uint16_t j = 0; j < 256; j++) {
			*((uint16_t*)buffer) = port16_in(ATA_DATA(base));
			buffer = ((uint16_t*)buffer) + 1;
		}
	}

	return sectors == 0x00 ? 0x0100 : sectors;
}

/*
bool ata_write28(uint16_t base, bool master, uint32_t address, const void *buffer, uint8_t sectors) {
	// TODO: https://wiki.osdev.org/ATA_PIO_Mode#x86_Directions
}
*/
