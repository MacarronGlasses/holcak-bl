#include <driver/ata.h>
#include "disk.h"

uint32_t disk_read(disk_t disk, uint64_t address, void *buffer, uint32_t sectors) {
	address += disk.begin;
	switch (disk.type) {
		case DISK_TYPE_ATA: {
			if (address & ~(UINT64_MAX >> 0x24)) {
				sectors = (sectors > UINT16_MAX ? 0x00 : sectors);
				return ata_read48(disk.data.ata.address, disk.data.ata.master, address, buffer, sectors);
			} else {
				sectors = (sectors > UINT8_MAX ? 0x00 : sectors);
				return ata_read28(disk.data.ata.address, disk.data.ata.master, address, buffer, sectors);
			}
		} break;
	}
}

uint32_t disk_write(disk_t disk, uint64_t address, const void *buffer, uint32_t sectors) {
	address += disk.begin;
	switch (disk.type) {
		case DISK_TYPE_ATA: {
			if (address & ~(UINT64_MAX << 0x1C)) {
				sectors = (sectors > UINT8_MAX ? 0x00 : sectors);
				return ata_write28(disk.data.ata.address, disk.data.ata.master, address, buffer, sectors);
			} else {
				sectors = (sectors > UINT16_MAX ? 0x00 : sectors);
				return ata_write48(disk.data.ata.address, disk.data.ata.master, address, buffer, sectors);
			}
		} break;
	}
	return 0x00;
}
