#ifndef DRIVER_ISR_H_
#define DRIVER_ISR_H_
#include <global.h>
#include <stdint.h>
#include "isrg.h"

typedef struct __packed {
	uint32_t ds, edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t id, error;
	uint32_t eip, cs, flags, user_esp, ss;
} isr_frame_t;

typedef void(*isr_handler_t)(isr_frame_t);

void isr_handler_init(uint8_t id, isr_handler_t handler);
__cdecl void isr_handler(isr_frame_t frame);

#endif//DRIVER_ISR_H_
