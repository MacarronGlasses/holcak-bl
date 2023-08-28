#ifndef STAGE2_PCI_H_
#define STAGE2_PCI_H_
#include <define.h>

typedef enum {
	PCI_MECHANISM_NOT_FOUND = 0x00,
	PCI_MECHANISM_CONFIG_SPACE_ACCESS_1 = 0x01,
	PCI_MECHANISM_CONFIG_SPACE_ACCESS_2 = 0x02,
	PCI_MECHANISM_SPECIAL_CYCLE_GENERATION_1 = 0x10,
	PCI_MECHANISM_SPECIAL_CYCLE_GENERATION_2 = 0x20,
} pci_mechanism_t;

typedef struct __packed {
	uint16_t version;
	uint8_t mechanism;
	uint8_t buses;
} pci_info_t;

__cdecl extern pci_info_t *pci_init(void);

#endif//STAGE2_PCI_H_
