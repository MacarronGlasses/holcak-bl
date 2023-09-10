#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <stdint.h>

#define __packed __attribute__((packed))
#define __cdecl  __attribute__((cdecl))

typedef struct __packed {
	uint8_t head;
	uint16_t sector : 6;
	uint16_t cylinder : 10;
} chs_t;

#endif//GLOBAL_H_
