#ifndef DRIVER_ATA_H_
#define DRIVER_ATA_H_
#include <stdbool.h>
#include <stdint.h>

#define ATA_DATA(Base)     ((Base) + 0x00)
#define ATA_ERROR(Base)    ((Base) + 0x01)
#define ATA_FEATURES(Base) ((Base) + 0x01)
#define ATA_SECTORS(Base)  ((Base) + 0x02)
#define ATA_LBA_LO(Base)   ((Base) + 0x03)
#define ATA_LBA_MI(Base)   ((Base) + 0x04)
#define ATA_LBA_HI(Base)   ((Base) + 0x05)
#define ATA_DEVICE(Base)   ((Base) + 0x06)
#define ATA_STATUS(Base)   ((Base) + 0x07)
#define ATA_COMMAND(Base)  ((Base) + 0x07)
#define ATA_CONTROL(Base)  ((Base) + 0x0206)

#define ATA_IDENTIFY 0xEC
#define ATA_FLUSH    0xE7
#define ATA_READ28   0x20
#define ATA_WRITE28  0x30

uint8_t ata_init(uint16_t base, bool master);
uint16_t ata_read28(uint16_t base, bool master, uint32_t address, void *buffer, uint8_t sectors);
uint16_t ata_write28(uint16_t base, bool master, uint32_t address, const void *buffer, uint8_t sectors);

#endif//DRIVER_ATA_H_
