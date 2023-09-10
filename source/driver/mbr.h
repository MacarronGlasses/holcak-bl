#ifndef DRIVER_MBR_H_
#define DRIVER_MBR_H_
#include <stdint.h>

typedef struct __packed {
	uint8_t bootable;
	chs_t chs_start;
	uint8_t system_id;
	chs_t chs_end;
	uint32_t address;
	uint32_t sectors;
} mbr_partition_t;

#endif//DRIVER_MBR_H_
