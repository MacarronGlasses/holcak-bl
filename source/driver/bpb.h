#ifndef DRIVER_BPB_H_
#define DRIVER_BPB_H_
#include <global.h>
#include <stdint.h>

typedef struct __packed {
	uint16_t bytes_per_sector;
	uint8_t sectors_per_cluster;
	uint16_t reserved_sectors;
	uint8_t number_of_fat;
	uint16_t root_entries;
	uint16_t total_sectors;
	uint8_t media_descriptor;
	uint16_t sectors_per_fat;
} bpb_dos20_t;

typedef struct __packed {
	bpb_dos20_t dos20;
	uint16_t sectors_per_track;
	uint16_t number_of_heads;
	uint16_t hidden_sectors;
} bpb_dos30_t;

typedef struct __packed {
	bpb_dos30_t dos30;
	uint16_t total_sectors;
} bpb_dos32_t;

typedef struct __packed {
	bpb_dos20_t dos20;
	uint16_t sectors_per_track;
	uint16_t number_of_heads;
	uint32_t hidden_sectors;
	uint32_t total_sectors;
} bpb_dos331_t;

typedef struct __packed {
	bpb_dos331_t dos331;
	uint8_t drive;
	uint8_t flags;
	uint8_t signature;
	uint32_t serial_number;
} bpb_dos34_t;

typedef struct __packed {
	bpb_dos331_t dos331;
	uint8_t drive;
	uint8_t flags;
	uint8_t signature;
	uint32_t serial_number;
	char label[0x0B];
	char type[0x08];
} bpb_dos40_t;

typedef struct __packed {
	bpb_dos331_t dos331;
	uint32_t sectors_per_fat;
	uint16_t mirror_flags;
	uint16_t version;
	uint32_t root_cluster;
	uint16_t info_sector;
	uint16_t backup_sector;
	char reserved[0x0C];
	uint8_t drive;
	uint8_t flags;
	uint8_t signature;
	uint32_t serial_number;
	char label[0x0B];
	char type[0x08];
} bpb_dos71_t;

typedef struct __packed {
	bpb_dos331_t dos331;
	uint8_t drive;
	uint8_t flags;
	uint8_t signature;
	uint8_t reserved;
	uint64_t sectors_in_volume;
	uint64_t mft_cluster;
	uint64_t mft_mirror_cluster;
	uint32_t mft_record_size;
	uint32_t index_block_size;
	uint64_t serial_number;
	uint32_t checksum;
} bpb_ntfs_t;

#endif//DRIVER_BPB_H_
