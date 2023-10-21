#pragma once
#include <stdbool.h>
#include <stdint.h>

// TODO: https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ata/ns-ata-_identify_device_data

typedef struct {
	uint16_t base;
	bool master;
} pata_t;

uint64_t pata_init(uint16_t base, bool master);
uint32_t pata_read(pata_t info, uint64_t address, void *buffer, uint32_t sectors);
uint32_t pata_write(pata_t info, uint64_t address, const void *buffer, uint32_t sectors);
