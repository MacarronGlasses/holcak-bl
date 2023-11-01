#pragma once
#include <global.h>
#include <stdint.h>

typedef struct __packed {
	uint32_t levels;
	char vendor[16];
	uint32_t features[4];
} cpu_info_t;

typedef struct __packed {
	uint32_t entry;
	uint16_t version;
	uint16_t buses;
	uint8_t hardware;
} pci_info_t;

extern cpu_info_t cpu_info;
extern pci_info_t pci_info;
