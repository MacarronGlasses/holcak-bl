#ifndef STAGE3_INFO_H_
#define STAGE3_INFO_H_
#include <define.h>
#include <stdint.h>

#define MEM_NODE_USABLE       0x01
#define MEM_NODE_RESERVED     0x02
#define MEM_NODE_ACPI_RECLAIM 0x03
#define MEM_NODE_ACPI_NVS     0x04
#define MEM_NODE_INVALID      0x05

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

typedef struct __packed {
	uint32_t base[2];
	uint32_t limit[2];
	uint32_t type;
	uint32_t acpi;
} mem_node_t;

typedef struct __packed {
	uint16_t length;
	uint16_t bytes;
	mem_node_t data[];
} mem_info_t;

extern cpu_info_t cpu_info;
extern pci_info_t pci_info;
extern mem_info_t mem_info;

#endif//STAGE3_INFO_H_
