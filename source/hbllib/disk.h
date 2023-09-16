#ifndef HBLLIB_DISK_H_
#define HBLLIB_DISK_H_
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	DISK_TYPE_ATA,
} disk_type_t;

typedef union {
	struct {
		uint16_t address;
		bool master;
	} ata;
} disk_data_t;

typedef struct {
	disk_type_t type;
	disk_data_t data;
	uint64_t begin;
	uint64_t limit;
} disk_t;

uint32_t disk_read(disk_t disk, uint64_t address, void *buffer, uint32_t sectors);
uint32_t disk_write(disk_t disk, uint64_t address, const void *buffer, uint32_t sectors);

#endif//HBLLIB_DISK_H_
