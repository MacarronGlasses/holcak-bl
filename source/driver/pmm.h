#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "mem.h"

#define PMM_PAGE_SIZE ((0x1000 - sizeof(size_t) - sizeof(struct pmm_page*)) / sizeof(struct pmm_node))

typedef enum pmm_type {
	PMM_TYPE_UNKNOWN,
	PMM_TYPE_FREE,
	PMM_TYPE_USED,
} pmm_type_t;

typedef struct pmm_node {
	struct pmm_node *next;
	size_t base;
	size_t size;
	pmm_type_t type;
} pmm_node_t;

typedef struct pmm_page {
	struct pmm_page *next;
	size_t size;
	pmm_node_t data[PMM_PAGE_SIZE];
} pmm_page_t;

_Static_assert(sizeof(pmm_page_t) <= 0x1000, "PMM page has to be able to fit inside a memory page!");

void pmm_dump(void);

bool pmm_init(const mem_info_t *info);
void *pmm_alloc(void);
void pmm_free(void *base);
