#pragma once
#include <global.h>
#include <stdint.h>
#include <wchar.h>
#include "bpb.h"

#define FAT_ENTRY_READ_ONLY 0x01
#define FAT_ENTRY_HIDDEN    0x02
#define FAT_ENTRY_SYSTEM    0x04
#define FAT_ENTRY_VOLUME_ID 0x08
#define FAT_ENTRY_FOLDER    0x10
#define FAT_ENTRY_ARCHIVE   0x20
#define FAT_ENTRY_LFN       (FAT_ENTRY_READ_ONLY | FAT_ENTRY_HIDDEN | FAT_ENTRY_SYSTEM | FAT_ENTRY_VOLUME_ID)

typedef struct __packed {
	uint16_t hour : 5;
	uint16_t minute : 6;
	uint16_t second : 5;
} fat_time_t;

typedef struct __packed {
	uint16_t year : 7;
	uint16_t month : 4;
	uint16_t day : 5;
} fat_date_t;

typedef struct __packed {
	char name[8];
	char extension[3];
	uint8_t flags;
	uint8_t reserved;
	uint8_t created_10ms;
	fat_time_t created_time;
	fat_date_t created_date;
	fat_date_t accessed_date;
	uint16_t cluster_hi;
	fat_time_t changed_time;
	fat_date_t changed_date;
	uint16_t cluster_lo;
	uint32_t size;
} fat_entry_t;

typedef struct __packed {
	uint8_t order;
	wchar_t data_lo[5];
	uint8_t flags;
	uint8_t type;
	uint8_t checksum;
	wchar_t data_mi[6];
	uint16_t reserved;
	wchar_t data_hi[2];
} fat_entry_lfn_t;
