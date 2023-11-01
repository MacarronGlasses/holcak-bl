#include "pmm.h"

static pmm_page_t __align(0x1000) pmm_page = {0};

static mem_node_t *pmm_node_alloc(void) {
	pmm_page_t *page = &pmm_page;
	while (true) {
		if (page->next == NULL) {
			page->next = pmm_alloc();
			if ((page = page->next) == NULL) {
				return NULL;
			}
			*page = (pmm_page_t){
				.next = NULL,
				.data = {0},
				.size = 1,
			};
			return &page->data[0];
		}
		else if (page->size == PMM_PAGE_SIZE) {
			page = page->next;
			continue;
		}
		for (size_t i = 0; i < PMM_PAGE_SIZE; i++) {
			if (page->data[i].type == 0) {
				return &page->data[i];
			}
		}
	}
}

static void pmm_node_free(pmm_node_t *self) {
	pmm_page_t *page = self & ~0x0FFF;
	*self = (pmm_node_t){
		.next = NULL,
		.base = 0,
		.size = 0,
		.type = 0,
	};
	page->size -= 1;
}

bool pmm_init(mem_info_t info) {
	for (size_t i = 0; i < info.size; i++) {
		pmm_node_t *self = pmm_node_alloc();
		if (self == NULL) {
			return false;
		}
		*self = (pmm_node_t){
			.next = NULL,
			.base = (info.data[i].base & ~0x0FFF) + ((info.data[i].base & 0x0FFF) && info.data[i].type == MEM_TYPE_USABLE),
			.size = (info.data[i].size & ~0x0FFF) + ((info.data[i].base & 0x0FFF) && info.data[i].type != MEM_TYPE_USABLE),
			.type = info.data[i].type,
		};
		if (self == &pmm_page.data[0]) {
			continue;
		}
		pmm_node_t *node = &pmm_page.data[0];
		while (node->next != NULL) {
			// TODO!
			node = node->next;
		}
		node->next = self;
	}
	return true;
}

void *pmm_alloc(void);
void pmm_free(void *base);