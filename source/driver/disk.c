#include "disk.h"

disk_t disk_pata(uint16_t base, bool master) {
	pata_info_t info = {base, master};
	if (!pata_init(info)) {
		return (disk_t){DISK_TYPE_ERROR, .data = {{0x00}}};
	}
	return (disk_t){DISK_TYPE_PATA, .data.pata = info};
}

uint32_t disk_read(disk_t disk, uint64_t address, void *buffer, uint32_t sectors) {
	switch (disk.type) {
		case DISK_TYPE_PATA: {
			return pata_read(disk.data.pata, address, buffer, sectors);
		} break;
		default: {
		} return 0x00;
	}
}

uint32_t disk_write(disk_t disk, uint64_t address, const void *buffer, uint32_t sectors) {
	switch (disk.type) {
		case DISK_TYPE_PATA: {
			return pata_write(disk.data.pata, address, buffer, sectors);
		} break;
		default: {
		} return 0x00;
	}
}
