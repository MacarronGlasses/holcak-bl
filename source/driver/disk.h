#ifndef STAGE3_DISK_H_
#define STAGE3_DISK_H_
#include <stdbool.h>
#include <stdint.h>
#include "pata.h"

typedef enum {
	DISK_TYPE_PATA,
} disk_type_t;

typedef union {
	pata_info_t pata;
} disk_data_t;

typedef struct {
	disk_type_t type;
	disk_data_t data;
	uint64_t address;
	uint64_t sectors;
} disk_t;

disk_t disk_pata(uint16_t base, bool master);
uint32_t disk_read(disk_t *disk, uint64_t address, void *buffer, uint32_t sectors);
uint32_t disk_write(disk_t *disk, uint64_t address, const void *buffer, uint32_t sectors);

#endif//STAGE3_DISK_H_
