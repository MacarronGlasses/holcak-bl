#include <global.h>
#include <string.h>
#include "disk.h"

disk_t disk_pata(uint16_t base, bool master) {
	return (disk_t){DISK_TYPE_PATA, .data.pata = {base, master}, 0x00, pata_init(base, master)};
}

uint32_t disk_read(disk_t *self, uint64_t begin, void *buffer, uint32_t limit) {
	limit = MIN(limit, self->limit - begin);
	if (begin > self->limit) {
		return 0x00;
	}
	begin += self->begin;
	if (begin < self->begin) {
		return 0x00;
	}
	switch (self->type) {
		case DISK_TYPE_PATA: return pata_read(self->data.pata, begin, buffer, limit);
	}
	return 0x00;
}

uint32_t disk_write(disk_t *self, uint64_t begin, const void *buffer, uint32_t limit) {
	limit = MIN(limit, self->limit - begin);
	if (begin > self->limit) {
		return 0x00;
	}
	begin += self->begin;
	if (begin < self->begin) {
		return 0x00;
	}
	switch (self->type) {
		case DISK_TYPE_PATA: return pata_write(self->data.pata, begin, buffer, limit);
	}
	return 0x00;
}
