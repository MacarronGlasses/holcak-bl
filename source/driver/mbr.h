#ifndef DRIVER_MBR_H_
#define DRIVER_MBR_H_
#include <global.h>
#include <stdint.h>

#define MBR_BEGIN 0x1BE

typedef struct __packed {
	uint8_t bootable;
	uint16_t begin_lo;
	uint8_t begin_hi;
	uint8_t system_id;
	uint16_t limit_lo;
	uint8_t limit_hi;
	uint32_t address;
	uint32_t sectors;
} mbr_partition_t;

#endif//DRIVER_MBR_H_
