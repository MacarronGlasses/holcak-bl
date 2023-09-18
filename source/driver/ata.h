#ifndef DRIVER_ATA_H_
#define DRIVER_ATA_H_
#include <stdbool.h>
#include <stdint.h>

// TODO: https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ata/ns-ata-_identify_device_data

bool ata_init(uint16_t base, bool master);
uint32_t ata_read(uint16_t base, bool master, uint64_t address, void *buffer, uint32_t sectors);
uint32_t ata_write(uint16_t base, bool master, uint64_t address, const void *buffer, uint32_t sectors);

#endif//DRIVER_ATA_H_
