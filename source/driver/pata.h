#ifndef DRIVER_PATA_H_
#define DRIVER_PATA_H_
#include <stdbool.h>
#include <stdint.h>

// TODO: https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ata/ns-ata-_identify_device_data

typedef struct {
	uint16_t base;
	bool master;
} pata_info_t;

bool pata_init(pata_info_t info);
uint32_t pata_read(pata_info_t info, uint64_t address, void *buffer, uint32_t sectors);
uint32_t pata_write(pata_info_t info, uint64_t address, const void *buffer, uint32_t sectors);

#endif//DRIVER_PATA_H_
