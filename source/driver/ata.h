#ifndef DRIVER_ATA_H_
#define DRIVER_ATA_H_
#include <stdbool.h>
#include <stdint.h>

#define ATA_COMMAND_IDENTIFY 0xEC
#define ATA_COMMAND_READ28   0x20
#define ATA_COMMAND_WRITE28  0x30
#define ATA_COMMAND_FLUSH28  0xE7
#define ATA_COMMAND_READ48   0x24
#define ATA_COMMAND_WRITE48  0x34
#define ATA_COMMAND_FLUSH48  0xEA

#define ATA_PORT_DATA(Base)        ((Base) + 0x00)
#define ATA_PORT_ERROR(Base)       ((Base) + 0x01)
#define ATA_PORT_FEATURES(Base)    ((Base) + 0x01)
#define ATA_PORT_SECTORS(Base)     ((Base) + 0x02)
#define ATA_PORT_ADDRESS_LO(Base)  ((Base) + 0x03)
#define ATA_PORT_ADDRESS_MI(Base)  ((Base) + 0x04)
#define ATA_PORT_ADDRESS_HI(Base)  ((Base) + 0x05)
#define ATA_PORT_DEVICE(Base)      ((Base) + 0x06)
#define ATA_PORT_STATUS(Base)      ((Base) + 0x07)
#define ATA_PORT_COMMAND(Base)     ((Base) + 0x07)
#define ATA_PORT_CONTROL(Base)     ((Base) + 0x0206)

// TODO: https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ata/ns-ata-_identify_device_data

uint8_t ata_init(uint16_t base, bool master);
uint16_t ata_read28(uint16_t base, bool master, uint32_t address, void *buffer, uint8_t sectors);
uint16_t ata_write28(uint16_t base, bool master, uint32_t address, const void *buffer, uint8_t sectors);
uint32_t ata_read48(uint16_t base, bool master, uint64_t address, void *buffer, uint16_t sectors);
uint32_t ata_write48(uint16_t base, bool master, uint64_t address, const void *buffer, uint16_t sectors);

#endif//DRIVER_ATA_H_
