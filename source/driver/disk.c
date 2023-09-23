#include <global.h>
#include <string.h>
#include "disk.h"

disk_t disk_pata(uint16_t base, bool master) {
	disk_t disk = {.type = DISK_TYPE_PATA, .data.pata = {base, master}};
	if (!pata_init(disk.data.pata)) {
		return (disk_t){.type = DISK_TYPE_ERROR};
	}
	char buffer[0x200];
	if (pata_read(disk.data.pata, 0x00, buffer, 0x01) != 0x01) {
		return (disk_t){.type = DISK_TYPE_ERROR};
	}
	memcpy(disk.partitions, &buffer[MBR_BEGIN], sizeof(mbr_partition_t) * 0x04);
	return disk;
}

uint32_t disk_read(disk_t *s, uint8_t partition, uint64_t address, void *buffer, uint32_t sectors) {
	address += s->partitions[partition].address;
	if (address < s->partitions[partition].address) {
		return 0x00;
	}
	sectors = MIN(sectors, s->partitions[partition].sectors);
	switch (s->type) {
		case DISK_TYPE_PATA: return pata_read(s->data.pata, address, buffer, sectors);
		default: return 0x00;
	}
}

uint32_t disk_write(disk_t *s, uint8_t partition, uint64_t address, const void *buffer, uint32_t sectors) {
	address += s->partitions[partition].address;
	if (address < s->partitions[partition].address) {
		return 0x00;
	}
	sectors = MIN(sectors, s->partitions[partition].sectors);
	switch (s->type) {
		case DISK_TYPE_PATA: return pata_write(s->data.pata, address, buffer, sectors);
		default: return 0x00;
	}
}
