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
		if (page->data[i].type == 0) {
			return &page->data[i];
		}
	}
	__builtin_unreachable();
}

#include <stage3/printf.h>

bool pmm_init(const mem_info_t *info) {
	for (size_t i = 0; i < info->size; i++) {
		const mem_node_t *node = &info->data[i];
		size_t base = node->base / 0x1000 + ((node->base & 0x0FFF) && node->type == MEM_TYPE_USABLE);
		if (node->base > SIZE_MAX) {
			continue;
		}
		size_t size = node->size;
		if (node->base + node->size > SIZE_MAX) {
			// TODO: Maybe add 1 to it?
			size = SIZE_MAX - node->base;
		}
		size = size / 0x1000 + ((size & 0x0FFF) && node->type != MEM_TYPE_USABLE);
		pmm_node_t *self = pmm_node_alloc();
		if (self == NULL) {
			return false;
		}
		*self = (pmm_node_t){
			.next = NULL,
			.base = base,
			.size = size,
			.type = node->type,
		};
		pmm_node_t *next = pmm_node;
		if (self == next) {
			continue;
		}
		if (self->base < next->base) {
			pmm_node = self;
			self->next = next;
			continue;
		}
		while (true) {
			// TODO: Handle overlapping!
			if (next->next == NULL) {
				next->next = self;
				break;
			}
			if (self->base < next->next->base) {
				self->next = next->next;
				next->next = self;
				break;
			}
			next = next->next;
		}
	}

	for (pmm_node_t *next = pmm_node; next != NULL; next = next->next) {
		printf("%x = %x (%x)\n", next->base, next->size, (uint32_t)next->type);
	}

	return true;
}

void *pmm_alloc(void) {
	return NULL;
}

void pmm_free(void *base) {
	(void)base;
}
