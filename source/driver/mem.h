#pragma once
#include <global.h>
#include <stdint.h>

typedef enum {
	MEM_TYPE_USABLE = 0x01,
	MEM_TYPE_RESERVED,
	MEM_TYPE_ACPI_CLAIM,
	MEM_TYPE_ACPI_NVS,
	MEM_TYPE_INVAILD,
} mem_type_t;

typedef struct __packed {
	uint64_t base;
	uint64_t size;
	uint32_t type;
	uint32_t acpi;
} mem_node_t;

typedef struct __packed {
	uint16_t size;
	uint16_t block;
	uint64_t total;
	mem_node_t data[];
} mem_info_t;
