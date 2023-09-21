#ifndef DRIVER_MBR_H_
#define DRIVER_MBR_H_
#include <stdint.h>

typedef struct __packed {
	uint8_t bootable;
	uint16_t begin;
	uint8_t system;
	uint16_t limit;
	uint32_t address;
	uint32_t sectors;
} mbr_part_t;

#endif//DRIVER_MBR_H_
