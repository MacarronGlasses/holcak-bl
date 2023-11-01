#pragma once
#include <global.h>
#include <stdint.h>
#include "mem.h"

// NOTE: https://www.gnu.org/software/grub/manual/multiboot/html_node/multiboot_002eh.html
// TODO: Make separate APM, ... header!

#define MB1_HEADER_MAGIC      0x1BADB002
#define MB1_BOOTLOADER_MAGIC  0x2BADB002

#define MB1_HEADER_ALIGN      0x04
#define MB1_MODULE_ALIGN      0x1000

#define MB1_FLAGS_ALIGN       0x01
#define MB1_FLAGS_MEMORY      0x02
#define MB1_FLAGS_VIDEO       0x04
#define MB1_FLAGS_AOUT_KLUDGE 0x010000

#define MB1_INFO_MEMORY       0x01
#define MB1_INFO_DEVICE       0x02
#define MB1_INFO_CMDLINE      0x04
#define MB1_INFO_MODULES      0x08

#define MB1_INFO_AOUT_SYMBOLS 0x10
#define MB1_INFO_ELF_SECTIONS 0x20

#define MB1_INFO_MMAP         0x40
#define MB1_INFO_DRIVE        0x80
#define MB1_INFO_CONFIG       0x0100
#define MB1_INFO_NAME         0x0200
#define MB1_INFO_APM_TABLE    0x0400

#define MB1_INFO_VBE          0x0800
#define MB1_INFO_FRAMEBUFFER  0x1000

#define MB1_HEADER_CHECKSUM(Self) ((Self).magic + (Self).flags + (Self).checksum)

#define MB1_FRAMEBUFFER_TYPE_INDEXED  0
#define MB1_FRAMEBUFFER_TYPE_RGB      1
#define MB1_FRAMEBUFFER_TYPE_EGA_TEXT 2

typedef struct __packed {
	uint32_t magic;
	uint32_t flags;
	uint32_t checksum;
	struct __packed {
		uint32_t header;
		uint32_t load;
		uint32_t load_end;
		uint32_t bss_end;
		uint32_t entry;
	} aout;
	struct __packed {
		uint32_t mode;
		uint32_t width;
		uint32_t height;
		uint32_t depth;
	} video;
} mb1_header_t;

typedef struct __packed {
	uint32_t tabsize;
	uint32_t strsize;
	uint32_t address;
	uint32_t reserved;
} mb1_aout_symbols_t;

typedef struct __packed {
	uint32_t number;
	uint32_t size;
	uint32_t address;
	uint32_t shndx;
} mb1_elf_sections_t;

typedef struct __packed {
	uint32_t flags;
	uint32_t mem_lower;
	uint32_t mem_upper;
	uint32_t device;
	uint32_t cmdline;
	uint32_t modules_size;
	uint32_t modules_address;
	union __packed {
		mb1_aout_symbols_t aout_symbols;
		mb1_elf_sections_t elf_sections;
	};
	uint32_t mmap_size;
	uint32_t mmap_address;
	uint32_t drives_size;
	uint32_t drives_address;
	uint32_t config;
	uint32_t name;
	uint32_t apm_table;
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint16_t vbe_mode;
	uint16_t vbe_interface_segment;
	uint16_t vbe_interface_offset;
	uint16_t vbe_interface_size;
	uint64_t framebuffer_address;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint8_t framebuffer_bpp;
	uint8_t framebuffer_type;
	union __packed {
		struct __packed {
			uint32_t framebuffer_palette_address;
			uint16_t framebuffer_palette_colors;
		};
		struct __packed {
			uint8_t framebuffer_red_field_position;
			uint8_t framebuffer_red_mask_size;
			uint8_t framebuffer_green_field_position;
			uint8_t framebuffer_green_mask_size;
			uint8_t framebuffer_blue_field_position;
			uint8_t framebuffer_blue_mask_size;
		};
	};
} mb1_info_t;

typedef struct __packed {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} mb1_color_t;

typedef struct __packed {
	uint32_t begin;
	uint32_t end;
	uint32_t cmdline;
	uint32_t padding;
} mb1_modules_t;

typedef struct __packed {
	uint16_t version;
	uint16_t cseg;
	uint32_t offset;
	uint16_t cseg_16;
	uint16_t dseg;
	uint16_t flags;
	uint16_t cseg_size;
	uint16_t cseg_16_size;
	uint16_t dseg_size;
} mb1_apm_info_t;
