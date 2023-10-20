#include <global.h>
#include <string.h>
#include "disk.h"

disk_t disk_pata(uint16_t base, bool master) {
	return (disk_t){DISK_TYPE_PATA, .data.pata = {base, master}, 0x00, pata_init(base, master)};
}

uint32_t disk_read(disk_t *disk, uint64_t address, void *buffer, uint32_t sectors) {
	sectors = MIN(sectors, disk->sectors - address);
	if (address > disk->sectors) {
		return 0x00;
	}
	address += disk->address;
	if (address < disk->address) {
		return 0x00;
	}
	switch (disk->type) {
		case DISK_TYPE_PATA: return pata_read(disk->data.pata, address, buffer, sectors);
	}
	return 0x00;
}

uint32_t disk_write(disk_t *disk, uint64_t address, const void *buffer, uint32_t sectors) {
	sectors = MIN(sectors, disk->sectors - address);
	if (address > disk->sectors) {
		return 0x00;
	}
	address += disk->address;
	if (address < disk->address) {
		return 0x00;
	}
	switch (disk->type) {
		case DISK_TYPE_PATA: return pata_write(disk->data.pata, address, buffer, sectors);
	}
	return 0x00;
}
