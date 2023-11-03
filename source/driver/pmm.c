#include <string.h>
#include "pmm.h"

static pmm_page_t __align(0x1000) pmm_page = {0};
static pmm_node_t *pmm_node = &pmm_page.data[0];

static pmm_node_t *pmm_node_alloc(void) {
	pmm_page_t *page = &pmm_page;
	while (page->size == PMM_PAGE_SIZE) {
		if (page->next == NULL) {
			if ((page = page->next = pmm_alloc()) == NULL) {
				return NULL;
			}
			*page = (pmm_page_t){
				.next = NULL,
				.size = 1,
			};
			memset(page->data, 0, PMM_PAGE_SIZE * sizeof(pmm_node_t));
			return &page->data[0];
		}
	}
	if (page->size == PMM_PAGE_SIZE - 1 && page->next == NULL) {
		if ((page = page->next = pmm_alloc()) == NULL) {
			return NULL;
		}
		*page = (pmm_page_t){
			.next = NULL,
			.size = 1,
		};
		memset(page->data, 0, PMM_PAGE_SIZE * sizeof(pmm_node_t));
		return &page->data[0];
	}
	for (size_t i = 0; i < PMM_PAGE_SIZE; i++) {
		if (page->data[i].type == PMM_TYPE_UNKNOWN) {
			return &page->data[i];
		}
	}
	__builtin_unreachable();
}

static void pmm_node_insert(pmm_node_t *self) {
	pmm_node_t *node = pmm_node;
	if (self == node) {
		return;
	}
	if (self->base < node->base) {
		pmm_node = self;
		self->next = node;
		return;
	}
	while (node->next != NULL) {
		if (self->base < node->next->base) {
			self->next = node->next;
			node->next = self;
			return;
		}
		node = node->next;
	}
	node->next = self;
}

#include <stage3/printf.h>

void pmm_dump(void) {
	for (pmm_node_t *node = pmm_node; node != NULL; node = node->next) {
		printf("%x = %x (%x)\n", node->base, node->size, (uint32_t)node->type);
	}
}

bool pmm_init(const mem_info_t *info) {
	// TODO: Handle overlapping!
	for (size_t i = 0; i < info->size; i++) {
		const mem_node_t *node = &info->data[i];
		if (node->type != MEM_TYPE_USABLE) {
			continue;
		}
		size_t base = node->base / 0x1000 + (bool)(node->base & 0x0FFF);
		if (node->base > SIZE_MAX) {
			continue;
		}
		size_t size = node->size;
		if (node->base + node->size > SIZE_MAX) {
			size = SIZE_MAX - node->base + 1;
		}
		size = size / 0x1000;
		pmm_node_t *self = pmm_node_alloc();
		if (self == NULL) {
			return false;
		}
		*self = (pmm_node_t){
			.next = NULL,
			.base = base,
			.size = size,
			.type = PMM_TYPE_FREE,
		};
		pmm_node_insert(self);
	}
	return true;
}

void *pmm_alloc(void) {
	pmm_node_t *prev = NULL;
	for (pmm_node_t *node = pmm_node; node != NULL; prev = node, node = node->next) {
		if (node->type == PMM_TYPE_USED) {
			continue;
		}
		if (node->next->type == PMM_TYPE_USED && node->next->base == node->base + node->size) {
			if (--node->size == 0) {
				prev == NULL ? (pmm_node = node->next) : (prev->next = node->next);
				node->type = PMM_TYPE_UNKNOWN;
			}
			node->next->base -= 1;
			node->next->size += 1;
			return (void*)(node->next->base * 0x1000);
		}
		if (node->size == 1) {
			node->type = PMM_TYPE_USED;
			return (void*)(node->base * 0x1000);
		}
		pmm_node_t *self = pmm_node_alloc();
		if (node->size == 1) {
			node->type = PMM_TYPE_USED;
			return (void*)(node->base * 0x1000);
		}
		void *page = (void*)((node->base + --node->size) * 0x1000);
		*self = (pmm_node_t){
			.next = node->next,
			.base = node->base + node->size,
			.size = 1,
			.type = PMM_TYPE_USED,
		};
		node->next = self;
		return page;
	}
	return NULL;
}

void pmm_free(void *base) {
	(void)base;
}
